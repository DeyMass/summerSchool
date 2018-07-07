//
// Created by michail on 07.07.18.
//

#include "../include/definitions.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

bool selectedDir(int currentWindow, struct dirent** rDir, struct dirent** lDir, int selectedPos)
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

int goodSort(const struct dirent **a, const struct dirent **b)
{
    if ((*a)->d_type == DT_DIR && (*b)->d_type == DT_DIR){
        return strcmp((*a)->d_name, (*b)->d_name);
    }
    if ((*a)->d_type == DT_DIR) return -1;
    if ((*b)->d_type == DT_DIR) return 1;
    return 0;
}

void memFree(struct dirent ***var, int size)
{
    int i;
    for(i = 0; i < size; i++)
        free((*var)[i]);
    free(*var);
}