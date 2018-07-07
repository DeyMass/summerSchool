//
// Created by michail on 05.07.18.
//

#include <pthread.h>
#include <stdio.h>

#define N 100
int indexes[N];

void wire(int* index)
{
    printf("%i\n",*index);
}

int main()
{
    pthread_t thread[N];
    for (int i = 1; i <= N; i++){
        indexes[i-1] = i;
        pthread_create(&thread[i-1], 0, (void*)wire, &indexes[i-1]);
    }
    for(int i = 0; i < N; i++){
        pthread_join(thread[i], NULL);
    }
}