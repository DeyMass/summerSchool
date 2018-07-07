//
// Created by michail on 07.07.18.
//
#ifndef PROJECT_DIRECTORY_FUNC_H
#define PROJECT_DIRECTORY_FUNC_H

#include <stdlib.h>
#include <stdbool.h>

void memFree(struct dirent ***var, int size);

int goodSort(const struct dirent **a, const struct dirent **b);

bool selectedDir(int currentWindow, struct dirent** rDir, struct dirent** lDir, int selectedPos);

#endif //PROJECT_DIRECTORY_FUNC_H
