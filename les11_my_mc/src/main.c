//
// Created by michail on 30.06.18.
//

#include <sys/ioctl.h>
#include <sys/stat.h>
#include "sys/types.h"
#include <fcntl.h>
#include <termio.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dirent.h"
#include <sys/wait.h>
#include <pthread.h>
#include "../include/definitions.h"

WINDOW *leftWindowBorder;
WINDOW *rightWindowBorder;
WINDOW *progressBarWindow;
int maxHeight;
int maxWidth;
off_t progressMax = 0;
off_t progressNow = 0;
int isExit = 0;
int oldPercentage = 0;
pthread_cond_t  start_progress_cond;
pthread_mutex_t mutex;

//Draw 2 box windows
void showInterface()
{
    int margin = maxWidth / 2;
    leftWindowBorder = newwin(maxHeight, margin, 0, 0);
    rightWindowBorder = newwin(maxHeight, margin, 0, margin);
    box(leftWindowBorder, 0, 0);
    box(rightWindowBorder, 0, 0);
    wmove(stdscr, maxHeight, 0);
    wprintw(stdscr, "CTRL + X to exit | F5 to copy");
    refresh();
    wrefresh(rightWindowBorder);
    wrefresh(leftWindowBorder);
}

void progressUpdate(void)
{
    while(1) {
        pthread_cond_wait(&start_progress_cond, &mutex);
        if (isExit) break;
        float percentage = 0;
        float fakePercentage = 0;
        int diff = 0;
        start_color();
        WINDOW *percent = derwin(progressBarWindow, 1, PROG_WIDTH - 2, 2, 1);
        init_pair(15, COLOR_GREEN, COLOR_BLACK);
        wmove(progressBarWindow, 3, 1);
        waddch(progressBarWindow, '[');
        wmove(progressBarWindow, 3, PROG_WIDTH - 2);
        waddch(progressBarWindow, ']');
        wmove(progressBarWindow, 3, 2);
        while (progressNow < progressMax) {
            percentage = ((float) progressNow / (float) progressMax) * (PROG_WIDTH - 4);
            fakePercentage = ((float) progressNow / (float) progressMax) * 100;
            wmove(percent, 0, 0);
            wprintw(percent, "%i | %i completed %i%%", progressNow, progressMax, (int) fakePercentage);
            wrefresh(percent);
            if (percentage != oldPercentage) {
                wmove(percent, 0, 0);
                //wprintw(percent, "Completed for %i%%", (int)percentage);
                wrefresh(percent);
                diff = percentage - oldPercentage;
                oldPercentage = percentage;
                wattron(progressBarWindow, COLOR_PAIR(15));
                for (int i = 0; i < diff; i++) {
                    waddch(progressBarWindow, '#');
                }
                wrefresh(progressBarWindow);
            }
        }
        oldPercentage = 0;
        progressMax = 0;
        delwin(percent);
        progressNow = 0;
    }
}

void copy(char* which, char* where)
{
    int fdOriginal, fdCopy;
    fdOriginal = open(which, O_RDONLY);
    struct stat st1;
    stat(which, &st1);
    chdir(where);
    progressMax = st1.st_size;
    pthread_cond_signal(&start_progress_cond);
    fdCopy = open(which, O_CREAT, S_IREAD | S_IWRITE);
    close(fdCopy);
    fdCopy = open(which, O_WRONLY);
    char buffer[COPY_BLOCK_SIZE];
    ssize_t readed = 1;
    while(readed > 0){
        readed = read(fdOriginal, &buffer, COPY_BLOCK_SIZE);
        if (readed == 0) break;
        write(fdCopy, &buffer, readed);
        progressNow += readed;
    }
    pthread_cond_destroy(&start_progress_cond);
    pthread_cond_init(&start_progress_cond, NULL);
    close(fdOriginal);
    close(fdCopy);
}

void drawProgressWindow(char* which, char* where)
{
    int margin = maxWidth / 2;
    progressBarWindow = newwin(PROG_HEIGHT, PROG_WIDTH, maxHeight / 2 - (PROG_HEIGHT / 2), margin - (PROG_WIDTH / 2));
    box(progressBarWindow, 0, 0);
    wmove(progressBarWindow, 1, 1);

    wprintw(progressBarWindow, "copying is in progress");
    wmove(progressBarWindow, 2, 1);
    wrefresh(progressBarWindow);
    copy(which, where);
    while(progressMax > 0);
    showInterface();
}

//comparator to sort directories first
int goodSort(const struct dirent **a, const struct dirent **b)
{
    if ((*a)->d_type == DT_DIR && (*b)->d_type == DT_DIR){
        return strcmp((*a)->d_name, (*b)->d_name);
    }
    if ((*a)->d_type == DT_DIR) return -1;
    if ((*b)->d_type == DT_DIR) return 1;
    return 0;
}

void removeWindow()
{
    endwin();
}

pid_t runProgram(char* progPath)
{
    pid_t sideProc = fork();
    switch (sideProc){
        case 0:
            execl(progPath, progPath, NULL);
            break;
        default:
            return sideProc;
    }
}

//function to redraw entries list in window
void redraw(WINDOW **win, struct dirent **dir, int size, int selected)
{
    wclear(*win);
    start_color();
    char path[250];
    wbkgd(*win, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    struct stat fileInfo;
    fileInfo.st_size = 0;
    for (int i = 0; i < size && i < selected; i++) {
        if(dir[i]->d_type == DT_DIR){
            wattron(*win, COLOR_PAIR(1));
            waddstr(*win, dir[i]->d_name);
        }else{
            realpath(dir[i]->d_name, path);
            fileInfo.st_size = 0;
            stat(path, &fileInfo);
            waddstr(*win, dir[i]->d_name);
            if (fileInfo.st_size == 0)
                printf("mark %s [%i]\n",dir[i]->d_name, i);
            wprintw(*win, " <%i>", fileInfo.st_size);
        }
        waddch(*win, '\n');
        wattroff(*win, COLOR_PAIR(1));
    }
    if (selected < size) {
        wattron(*win, A_STANDOUT);
        if (dir[selected]->d_type == DT_DIR) {
            waddstr(*win, dir[selected]->d_name);
        } else{
            waddstr(*win, dir[selected]->d_name);
            realpath(dir[selected]->d_name, path);
            fileInfo.st_size = 0;
            stat(path, &fileInfo);
            wprintw(*win, " <%i>", fileInfo.st_size);
        }
        waddch(*win, '\n');
        wattroff(*win, A_STANDOUT);
        for (int i = selected + 1; i < size; i++) {
            if(dir[i]->d_type == DT_DIR){
                wattron(*win, COLOR_PAIR(1));
                waddstr(*win, dir[i]->d_name);
            }else{
                realpath(dir[i]->d_name, path);
                fileInfo.st_size = 0;
                stat(path, &fileInfo);
                waddstr(*win, dir[i]->d_name);
                wprintw(*win, " <%i>", fileInfo.st_size);
            }
            waddch(*win, '\n');
            wattroff(*win, COLOR_PAIR(1));
        }

    }
    refresh();
    wrefresh(*win);
}

bool isDirSelected(int currentWindow, struct dirent **rDir, struct dirent **lDir, int selectedPos)
{
    switch (currentWindow){
        case LEFT_WINDOW:
            if (lDir[selectedPos]->d_type != DT_DIR) return false;
            break;
        case RIGHT_WINDOW:
            if(rDir[selectedPos]->d_type != DT_DIR) return false;
    }
    return true;
}

//release memory allocated in 'scandir'
void memFree(struct dirent ***var, int size)
{
    int i;
    for(i = 0; i < size; i++)
        free((*var)[i]);
    free(*var);
}

int main(){
    pthread_cond_init(&start_progress_cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_t updater_pid;
    pthread_create(&updater_pid, NULL, (void*) progressUpdate, NULL);

    //Get window size
    struct winsize *size = malloc(sizeof(struct winsize));
    ioctl(0, TIOCGWINSZ, size);
    //sometimes size is undefined, because something is wrong.
    //can't do anything
    maxWidth = size->ws_col;
    maxHeight = size->ws_row - 1;
    free(size);
    int margin = maxWidth / 2;

    initscr();
    showInterface();
    WINDOW *leftDataWindow = newwin(maxHeight - 2, margin - 2, 1, 1);
    WINDOW *rightDataWindow = newwin(maxHeight - 2, margin - 2, 1, margin + 1);
    struct dirent **leftDir, **rightDir;
    char leftDirPath[250];
    char rightDirPath[250];
    int  selectedPosition = 0;
    int  isInterrupted = 0;
    //calculate number of entries in current directory
    char path[999] = ".";
    int  rDirSize = scandir(path, &rightDir, NULL, goodSort);
    int  lDirSize = scandir(path, &leftDir, NULL, goodSort);
    pid_t subroutine_pid;

    bool currentWindowType = LEFT_WINDOW;
    nodelay(leftDataWindow, true);//Enabled handling
    keypad(leftDataWindow, true); //for arrow
    keypad(stdscr, true);         //keys in
    keypad(rightDataWindow, true);//all windows
    curs_set(false);
    cbreak();

    int child_status;
    realpath(leftDir[selectedPosition]->d_name, leftDirPath);
    realpath(rightDir[selectedPosition]->d_name, rightDirPath);
    redraw(&rightDataWindow, rightDir, rDirSize, rDirSize);
    redraw(&leftDataWindow, leftDir, lDirSize, selectedPosition);

    while(!isInterrupted){
        noecho();
        sync();
        if (currentWindowType == LEFT_WINDOW) {
            redraw(&leftDataWindow, leftDir, lDirSize, selectedPosition);
        }
        else{
            redraw(&rightDataWindow, rightDir, rDirSize, selectedPosition);
        }

        int sym = getch();
        switch (sym) {
            case '\n': {
                if (isDirSelected(currentWindowType, rightDir, leftDir, selectedPosition) == false) {
                    if (currentWindowType == LEFT_WINDOW)
                        realpath(leftDir[selectedPosition]->d_name, path);
                    else
                        realpath(rightDir[selectedPosition]->d_name, path);
                    removeWindow();
                    subroutine_pid = runProgram(path);
                    wait(&child_status);
                    path[0] = '.';
                    path[1] = 0;
                    break;
                }
                if (currentWindowType == LEFT_WINDOW) {
                    realpath(leftDir[selectedPosition]->d_name, leftDirPath);
                    chdir(leftDirPath);
                    memFree(&leftDir, lDirSize);
                    lDirSize = scandir(path, &leftDir, NULL, goodSort);
                } else {
                    realpath(rightDir[selectedPosition]->d_name, rightDirPath);
                    chdir(rightDirPath);
                    memFree(&rightDir, rDirSize);
                    rDirSize = scandir(path, &rightDir, NULL, goodSort);
                }
                selectedPosition = 0;
                break;
            }
            case '\t': {
                selectedPosition = 0;
                //When switch from LEFT window to RIGHT window
                if (currentWindowType == LEFT_WINDOW) {
                    currentWindowType = RIGHT_WINDOW;
                    memFree(&leftDir, lDirSize);
                    lDirSize = scandir(leftDirPath, &leftDir, NULL, goodSort);
                    //remove highlight from string
                    redraw(&leftDataWindow, leftDir, lDirSize, lDirSize);
                    chdir(rightDirPath);
                }
                //When switch from RIGHT window to LEFT window
                else {
                    currentWindowType = LEFT_WINDOW;
                    memFree(&rightDir, rDirSize);
                    rDirSize = scandir(rightDirPath, &rightDir, NULL, goodSort);
                    //remove highlight from string
                    redraw(&rightDataWindow, rightDir, rDirSize, rDirSize);
                    chdir(leftDirPath);
                }
                break;
            }
            case KEY_DOWN: {
                //Check highlighted string in left window
                if (currentWindowType == LEFT_WINDOW && selectedPosition == lDirSize - 1) {
                    break;
                }
                    //Check highlighted string in right window
                else if (currentWindowType == RIGHT_WINDOW && selectedPosition == rDirSize - 1) {
                    break;
                }
                selectedPosition++;
                break;
            }
            case KEY_UP: {
                if (selectedPosition == 0) {
                    break;
                }
                selectedPosition--;
                break;
            }
            case KEY_F(5): {
                if (currentWindowType == LEFT_WINDOW) {
                    drawProgressWindow(leftDir[selectedPosition]->d_name, rightDirPath);
                    chdir(leftDirPath);
                    memFree(&rightDir, rDirSize);
                    rDirSize = scandir(rightDirPath, &rightDir, NULL, goodSort);
                    redraw(&rightDataWindow, rightDir, rDirSize, rDirSize);
                    redraw(&leftDataWindow, leftDir, lDirSize, selectedPosition);
                }
                if (currentWindowType == RIGHT_WINDOW) {
                    drawProgressWindow(rightDir[selectedPosition]->d_name, leftDirPath);
                    chdir(rightDirPath);
                    memFree(&leftDir, lDirSize);
                    lDirSize = scandir(leftDirPath, &leftDir, NULL, goodSort);
                    redraw(&leftDataWindow, leftDir, lDirSize, lDirSize);
                    redraw(&rightDataWindow, rightDir, rDirSize, selectedPosition);
                }

                break;
            }
            case CTRL('X'): {
                isInterrupted = 1;
                isExit = 1;
                pthread_cond_broadcast(&start_progress_cond);
                break;
            }
        }
    }
    pthread_join(updater_pid, NULL);
    memFree(&rightDir, rDirSize);
    memFree(&leftDir, lDirSize);
    delwin(rightDataWindow);
    delwin(leftDataWindow);
    delwin(rightWindowBorder);
    delwin(leftWindowBorder);
    delwin(stdscr);
    endwin();
}
