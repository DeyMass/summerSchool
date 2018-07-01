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

//Std hint window content
void clearHint(WINDOW* hint){
    wclear(hint);
    wprintw(hint, "ctrl+W Open\nctrl+E Save\nctrl+R Exit\n");//STD hint message
    wrefresh(hint);
}

//Save text in 'win' in file descriptor 'fd'
void funcWrite(WINDOW* win, int winHeight, int fd){
    char buffer[999];
    int i;
    for(i = 0; i < winHeight; i++) {
        wmove(win, i, 0);
        winstr(win, buffer);
        write(fd, buffer, strlen(buffer));
        buffer[0] = '\n';
        write(fd, buffer, 1);
        sync();
    }
}

//Read file in 'win' window from file descriptor 'fd'
void winRead(WINDOW* win, int fd){
    wclear(win);
    char buffer[15];
    sync();
    noecho();
    ssize_t readedSym = 1;
    attron(A_BLINK);
    while(readedSym > 0){
        sync();
        readedSym = read(fd, &buffer, 13);
        if (readedSym == 0) break;
        buffer[readedSym] = 0;
        wprintw(win, "%s", buffer);
        buffer[0] = 0;
    }
    wrefresh(win);
}

int main(int argc, char* argv[])
{
    const int width = 50;
    const int height = 20;
    struct winsize size;
    ioctl(0, TIOCGWINSZ, &size);
    unsigned short maxHeight = size.ws_row;
    unsigned short maxWidth = size.ws_col;
    initscr();
    int mainHeight;
    int mainWidth;
    int hintHeight;
    int hintWidth;
    //Content window count as maximum height minus hint content window and minus 4 border lines
    mainHeight = maxHeight - (HINT_WINDOW_HEIGHT + 4);
    mainWidth = maxWidth - 2;
    hintHeight = HINT_WINDOW_HEIGHT;
    hintWidth = maxWidth - 2;
    WINDOW *hintBorder = newwin(HINT_WINDOW_HEIGHT + 2, maxWidth, maxHeight - (HINT_WINDOW_HEIGHT + 2), 0);
    WINDOW *mainBorder = newwin(maxHeight - (HINT_WINDOW_HEIGHT + 2), maxWidth, 0, 0);
    WINDOW *main = newwin(mainHeight, mainWidth, 1, 1);
    WINDOW *hint = newwin(hintHeight, hintWidth, mainHeight + 3, 1);
    box(mainBorder, 0, 0);
    box(hintBorder, 0, 0);
    refresh();
    wrefresh(mainBorder);
    wrefresh(hintBorder);
    clearHint(hint);
    keypad(stdscr, true);
    keypad(main, true);
    keypad(hint, true);
    cbreak();
    char openFilename[MAX_NAME_LENGTH];
    char saveFilename[MAX_NAME_LENGTH];
    int  sym;
    int  fd;
    int  isInterrupted = 0;
    int  tickX = 0, tickY = 0;
    //set main window as current
    wprintw(main, "");
    wrefresh(main);
    //open file if file is given
    if (argc == 2) {
        fd = open(argv[1], O_RDONLY);
        winRead(main, fd);
        close(fd);
    }
    //-------------------------

    while(!isInterrupted){
        noecho();
        sync();
        sym = getch();
        switch (sym) {
            case KEY_DOWN:
                tickX++;
                //mainHeight - 1, because tickX count from 0
                tickX = tickX >= mainHeight - 1 ? mainHeight - 1 : tickX;
                wmove(main, tickX, tickY);
                break;
            case KEY_UP:
                if (tickX == 0){
                    break;
                }
                tickX--;
                wmove(main, tickX, tickY);
                break;
            case KEY_LEFT:
                if (tickY == 0){
                    break;
                }
                tickY--;
                wmove(main, tickX, tickY);
                break;
            case KEY_RIGHT:
                if (tickY == maxWidth - 3){
                    break;
                }
                tickY++;
                wmove(main, tickX, tickY);
                break;
            case KEY_BACKSPACE:
                tickY--;
                if (tickY < 0) tickY = 0;
                wmove(main, tickX, tickY);
                waddch(main, ' ');
                wmove(main, tickX, tickY);
                break;
            case CTRL('W'): //OPEN
                echo();
                clearHint(hint);
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
                noecho();
                wmove(main, 0, 0);
                close(fd);
                break;
            case CTRL('E')://SAVE
                echo();
                clearHint(hint);
                wprintw(hint, "Save as: ");
                wscanw(hint, "%s", saveFilename);
                wrefresh(hint);
                fd = open(saveFilename, O_RDWR | O_TRUNC | O_CREAT, S_IWRITE | S_IREAD);
                if (fd < 0){
                    wprintw(hint, " Error while creating file %s", saveFilename);
                    break;
                }
                wprintw(hint, "File %s created successfully", saveFilename);
                wrefresh(hint);
                funcWrite(main, maxHeight, fd);
                close(fd);
                break;
            case CTRL('R')://EXIT
                wprintw(main, "Astelavista");
                isInterrupted = 1;
                break;
            case '\n':
                tickX++;
                if (tickX == maxWidth - 1){
                    tickX = maxWidth - 1;
                }
                tickY = 0;
                wprintw(main, "\n");
                break;
            default:
                tickY++;
                wprintw(main, "%c", sym);
                if(tickY >= maxWidth - 2)
                    break;
                wmove(main, tickX, tickY);
        }
        wrefresh(hint);
        wrefresh(main);
    }
    endwin();

}
