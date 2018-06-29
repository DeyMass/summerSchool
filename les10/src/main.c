#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termio.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NAME_LENGTH    55
#define HINT_WINDOW_HEIGHT 4

void clearHint(WINDOW* hint){
    wclear(hint);
    wprintw(hint, "ctrl+W Open\nctrl+E Save\nctr+R Exit\n");
    wrefresh(hint);
}

void funcWrite(WINDOW* win, int fd){
    char buffer[999];
    winstr(win, buffer);
    write(fd, buffer, strlen(buffer));
}

void winRead(WINDOW* win, int fd){
    char buffer[15];
    int i = 0;
    int readedSym = 1;
    while(readedSym >0){
        readedSym = read(fd, &buffer, 14);
        buffer[readedSym+1] = 0;
        wprintw(win, "%s", buffer);
    }
    wrefresh(win);
}

int main()
{
    const int width = 50;
    const int height = 20;
    struct winsize size;
    ioctl(0, TIOCGWINSZ, &size);
    unsigned short maxHeight = size.ws_row;
    unsigned short maxWidth = size.ws_col;
    printf("maxHeight = %d, maxWidth = %d\n",maxHeight, maxWidth);
    initscr();
    WINDOW *subwindow = newwin(HINT_WINDOW_HEIGHT + 2, maxWidth, maxHeight - (HINT_WINDOW_HEIGHT + 2), 0);
    WINDOW *mainBorder = newwin(maxHeight - (HINT_WINDOW_HEIGHT + 2), maxWidth, 0, 0);
    WINDOW *main = newwin(maxHeight - (HINT_WINDOW_HEIGHT + 4), maxWidth - 2, 1, 1);
    WINDOW *hint = newwin(HINT_WINDOW_HEIGHT, maxWidth-2, maxHeight - (HINT_WINDOW_HEIGHT + 1), 1);
    box(mainBorder, 0, 0);
    box(subwindow, 0, 0);
    refresh();
    wrefresh(mainBorder);
    wrefresh(subwindow);
    clearHint(hint);
    sync();
    keypad(stdscr, true);
    keypad(main, true);
    keypad(hint, true);
    int sym;
    char openFilename[MAX_NAME_LENGTH];
    char saveFilename[MAX_NAME_LENGTH];
    int fd;
    cbreak();
    int isInterrupted = 0;
    int tickX = 0, tickY = 0;
    while(!isInterrupted){
        noecho();
        sync();
        sym = getch();
        switch (sym) {
            case CTRL('W'): //OPEN
                echo();
                clearHint(hint);
                wprintw(main, "read");
                wprintw(hint, "File to open: ");
                wscanw(hint, "%s", openFilename);
                wprintw(hint, "Reading %s ", openFilename);
                fd = open(openFilename, O_RDONLY);
                if (fd < 0) {
                    wprintw(hint, "FAILED");
                    break;
                }
                wprintw(hint, "SUCCEEDED");
                winRead(main, fd);
                wmove(main, 0, 0);
                close(fd);
                break;

            case KEY_DOWN:

                if (tickX == maxWidth - 2){
                    break;
                }
                tickX++;
                wmove(main, tickX, tickY);
                clearHint(hint);
                wprintw(hint, "x = %i y = %i", tickX, tickY);
                break;
            case KEY_UP:
                if (tickX == 0){
                    break;
                }
                tickX--;
                wmove(main, tickX, tickY);
                clearHint(hint);
                wprintw(hint, "x = %i y = %i", tickX, tickY);
                break;
            case KEY_LEFT:
                if (tickY == 0){
                    break;
                }
                tickY--;
                wmove(main, tickX, tickY);
                clearHint(hint);
                wprintw(hint, "x = %i y = %i", tickX, tickY);
                break;
            case KEY_RIGHT:
                if (tickY == maxWidth - 3){
                    break;
                }
                tickY++;
                wmove(main, tickX, tickY);
                clearHint(hint);
                wprintw(hint, "x = %i y = %i", tickX, tickY);
                break;
            case KEY_BACKSPACE:
                break;
            case CTRL('E')://SAVE
                echo();
                clearHint(hint);
                wprintw(hint, "Save as: ");
                wscanw(hint, "%s", saveFilename);
                fd = open(saveFilename, O_RDWR | O_TRUNC | O_CREAT);
                if (fd < 0){
                    wprintw(hint, " Error while creating a file");
                    break;
                }
                wprintw(hint, " filed created");
                funcWrite(main, fd);
                close(fd);
                break;
            case CTRL('R')://EXIT
                wprintw(main, "Astelavista");
                isInterrupted = 1;
                break;
            default:
                wprintw(main, "%c", sym);
        }
        wrefresh(hint);
        wrefresh(main);
    }
    endwin();

}
