//
// Created by michail on 07.07.18.
//

#include "ncurses.h"

//Draw 2 windows
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