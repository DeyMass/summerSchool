//
// Created by michail on 30.06.18.
//

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termio.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dirent.h"

#define LEFT_WINDOW  0
#define RIGHT_WINDOW 1

WINDOW *leftWindowBorder;
WINDOW *rightWindowBorder;
int maxHeight;
int maxWidth;

//Draw 2 windows
void showInterface(){
    int margin = maxWidth / 2;
    leftWindowBorder = newwin(maxHeight, margin, 0, 0);
    rightWindowBorder = newwin(maxHeight, margin, 0, margin);
    box(leftWindowBorder, 0, 0);
    box(rightWindowBorder, 0, 0);
    refresh();
    wrefresh(rightWindowBorder);
    wrefresh(leftWindowBorder);
}

//comparator to sort directories first
int goodSort(const struct dirent **a, const struct dirent **b){
    if ((*a)->d_type == DT_DIR && (*b)->d_type == DT_DIR){
        return strcmp((*a)->d_name, (*b)->d_name);
    }
    if ((*a)->d_type == DT_DIR) return -1;
    if ((*b)->d_type == DT_DIR) return 1;
    return 0;
}

//function to redraw entries list in window
void redraw(WINDOW **win, struct dirent **dir, int size, int selected){
    wclear(*win);
    start_color();
    wbkgd(*win, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    for (int i = 0; i < size && i < selected; i++) {
        if(dir[i]->d_type == DT_DIR) wattron(*win, COLOR_PAIR(1));
        waddstr(*win, dir[i]->d_name);
        waddch(*win, '\n');
        wattroff(*win, COLOR_PAIR(1));
    }
    if (selected < size) {
        wattron(*win, A_STANDOUT);
        waddstr(*win, dir[selected]->d_name);
        waddch(*win, '\n');
        wattroff(*win, A_STANDOUT);
        for (int i = selected + 1; i < size; i++) {
            if(dir[i]->d_type == DT_DIR) wattron(*win, COLOR_PAIR(1));
            waddstr(*win, dir[i]->d_name);
            waddch(*win, '\n');
            wattroff(*win, COLOR_PAIR(1));
        }

    }
    refresh();
    wrefresh(*win);
}

//release memory because 'scandir'...
void memFree(struct dirent ***var, int size){
    int i;
    for(i = 0; i < size; i++)
        free((*var)[i]);
    free(*var);
}

int main(){
    //Get window size
    struct winsize size;
    ioctl(0, TIOCGWINSZ, &size);
    char path[999] = ".";
    maxWidth = size.ws_col;
    maxHeight = size.ws_row;
    int margin = maxWidth / 2;

    initscr();
    showInterface();
    WINDOW *leftData = newwin(maxHeight - 2, margin - 2, 1, 1);
    WINDOW *rightData = newwin(maxHeight - 2, margin - 2, 1, margin + 1);
    struct dirent **leftDir, **rightDir;
    char leftDirPath[250];
    char rightDirPath[250];
    int  selectedPosition = 0;
    int  isInterrupted = 0;
    //calculate number of entries in current directory
    int  rDirSize = scandir(path, &rightDir, NULL, goodSort);
    int  lDirSize = scandir(path, &leftDir, NULL, goodSort);

    bool currentWindowType = LEFT_WINDOW;
    nodelay(leftData, true);//Enabled handling
    keypad(leftData, true); //for arrow
    keypad(stdscr, true);   //keys in
    keypad(rightData, true);//all windows
    curs_set(false);
    cbreak();


    realpath(leftDir[selectedPosition]->d_name, leftDirPath);
    realpath(rightDir[selectedPosition]->d_name, rightDirPath);
    redraw(&rightData, rightDir, rDirSize, rDirSize);
    redraw(&leftData, leftDir, lDirSize, selectedPosition);

    while(!isInterrupted){
        noecho();
        sync();
        if (currentWindowType == LEFT_WINDOW) {
            redraw(&leftData, leftDir, lDirSize, selectedPosition);
        }
        else{
            redraw(&rightData, rightDir, rDirSize, selectedPosition);
        }

        int sym = getch();
        switch (sym) {
            case '\n':
                if (currentWindowType == LEFT_WINDOW) {
                    realpath(leftDir[selectedPosition]->d_name, leftDirPath);
                    chdir(leftDirPath);
                    memFree(&leftDir, lDirSize);
                    lDirSize = scandir(path, &leftDir, NULL, goodSort);
                }
                else{
                    realpath(rightDir[selectedPosition]->d_name, rightDirPath);
                    chdir(rightDirPath);
                    memFree(&rightDir, rDirSize);
                    rDirSize = scandir(path, &rightDir, NULL, goodSort);
                }
                selectedPosition = 0;
                break;
            case '\t':
                selectedPosition = 0;
                //When switch from LEFT window to RIGHT window
                if (currentWindowType == LEFT_WINDOW) {
                    currentWindowType = RIGHT_WINDOW;
                    chdir(rightDirPath);
                    //remove highlight from string
                    redraw(&leftData, leftDir, lDirSize, lDirSize);
                }
                //When switch from RIGHT window to LEFT window
                else {
                    currentWindowType = LEFT_WINDOW;
                    chdir(leftDirPath);
                    //remove highlight from string
                    redraw(&rightData, rightDir, rDirSize, rDirSize);
                }
                break;
            case KEY_DOWN:
                //Check highlighted string in left window
                if (currentWindowType == LEFT_WINDOW && selectedPosition == lDirSize - 1){
                    break;
                }
                //Check highlighted string in right window
                else if(currentWindowType == RIGHT_WINDOW && selectedPosition == rDirSize - 1){
                    break;
                }
                selectedPosition++;
                break;
            case KEY_UP:
                if (selectedPosition == 0){
                    break;
                }
                selectedPosition--;
                break;
            case CTRL('X'):
                isInterrupted = 1;
                break;
        }
    }
    memFree(&rightDir, rDirSize);
    memFree(&leftDir, lDirSize);
    delwin(rightData);
    delwin(leftData);
    delwin(rightWindowBorder);
    delwin(leftWindowBorder);
    delwin(stdscr);

    getch();
    endwin();
}