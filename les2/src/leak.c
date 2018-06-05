#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *ptr  = NULL;
    for (int i = 0; i < 10; i++){
        ptr = (int*)malloc(sizeof(int) * 100);
        ptr[15] = 123;
    }
    printf("%i",ptr[150]);
    free(ptr);
}
