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

int goodSort(const struct dirent **a, const struct dirent **b){
    if ((*a)->d_type == DT_DIR && (*b)->d_type == DT_DIR){
        return strcmp((*a)->d_name, (*b)->d_name);
    }
    if ((*a)->d_type == DT_DIR) return -1;
    if ((*b)->d_type == DT_DIR) return 1;
    return 0;
}

void redraw(WINDOW **win, struct dirent **dir, int size, int selected){
    wclear(*win);
    for (int i = 0; i < size && i < selected; i++) {
        waddstr(*win, dir[i]->d_name);
        waddch(*win, '\n');
    }
    if (selected < size) {
        wattron(*win, A_STANDOUT);
        waddstr(*win, dir[selected]->d_name);
        waddch(*win, '\n');
        wattroff(*win, A_STANDOUT);
        for (int i = selected + 1; i < size; i++) {
            waddstr(*win, dir[i]->d_name);
            waddch(*win, '\n');
        }
    }

    refresh();
    wrefresh(*win);
}

void toggle(bool *var){
    *var = !(*var);
}

int main(){
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
    int rDirSize = scandir(path, &rightDir, NULL, goodSort);
    int lDirSize = scandir(path, &leftDir, NULL, goodSort);
    refresh();
    wrefresh(rightData);
    wrefresh(leftData);

    int selectedPosition = 0;
    int isInterrupted = 0;
    bool currentWindowType = LEFT_WINDOW;
    nodelay(leftData, true);
    keypad(leftData, true);
    keypad(stdscr, true);
    keypad(rightData, true);
    curs_set(false);
    cbreak();
    while(!isInterrupted){
        noecho();
        sync();
        wprintw(rightData, "cur win = %i", currentWindowType);
        if (currentWindowType == LEFT_WINDOW) {
            redraw(&leftData, leftDir, lDirSize, selectedPosition);
        }
        else{
            curscr = rightData;
            redraw(&rightData, rightDir, rDirSize, selectedPosition);
        }
        int sym = getch();
        switch (sym) {
            case '\n':
                return -1;
            case '\t':
                selectedPosition = 0;
                if (currentWindowType == LEFT_WINDOW) {
                    currentWindowType = RIGHT_WINDOW;
                    redraw(&leftData, leftDir, lDirSize, lDirSize);
                }
                else {
                    currentWindowType = LEFT_WINDOW;
                    redraw(&rightData, rightDir, rDirSize, rDirSize);
                }
                break;
            case 's':
                if (selectedPosition == lDirSize - 1){
                    break;
                }
                selectedPosition++;
                break;
            case 'w':
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


    getch();
    endwin();
}