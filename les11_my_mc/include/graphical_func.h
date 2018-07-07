//
// Created by michail on 07.07.18.
//

#ifndef PROJECT_GRAPHICAL_FUNC_H
#define PROJECT_GRAPHICAL_FUNC_H

#include <ncurses.h>

void redraw(WINDOW **win, struct dirent **dir, int size, int selected);

void drawProgressWindow(char* which, char* where);

void showInterface();

void progressUpdate(void);

void removeWindow();

#endif //PROJECT_GRAPHICAL_FUNC_H
