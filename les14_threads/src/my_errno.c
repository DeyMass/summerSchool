//
// Created by michail on 07.07.18.
//

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

pthread_once_t once;
pthread_key_t  key;

void init_key(){
    pthread_key_create(&key, NULL);
}

char* errno(int no){
    pthread_once(&once, init_key);
    char *buff;
    buff = pthread_getspecific(key);
    if (buff == NULL){
        buff = malloc(250 * sizeof(char));
        printf("allocated memory on %x\n", buff);
        pthread_setspecific(key, buff);
    }
    switch (no){
        case 0:
            buff[0] = 'c';
            buff[1] = 'a';
            buff[2] = 's';
            buff[3] = 'e';
            buff[4] = ' ';
            buff[5] = '0';
            buff[6] = 0;
            break;
        case 1:
            buff[0] = 'c';
            buff[1] = 'a';
            buff[2] = 's';
            buff[3] = 'e';
            buff[4] = ' ';
            buff[5] = '1';
            buff[6] = 0;
            break;
        case 2:
            buff[0] = 'c';
            buff[1] = 'a';
            buff[2] = 's';
            buff[3] = 'e';
            buff[4] = ' ';
            buff[5] = '2';
            buff[6] = 0;
            break;
        case 3:
            buff[0] = 'c';
            buff[1] = 'a';
            buff[2] = 's';
            buff[3] = 'e';
            buff[4] = ' ';
            buff[5] = '3';
            buff[6] = 0;
            break;
        default:
            buff[0] = 'c';
            buff[1] = 'a';
            buff[2] = 's';
            buff[3] = 'e';
            buff[4] = ' ';
            buff[5] = 'D';
            buff[6] = 0;
    }
    return buff;
}

void thread(int *myIndex)
{
    char* buff;
    for(int i = 0; i < 5; i++) {
        buff = errno(i);
        printf("thread %i has got %s buffer on %x address\n", *myIndex, buff, buff);
    }
    //закомментил для наглядности, но спровоцировал утечку памяти
    //free(buff);
}

int main(){
    pthread_t pid[3];
    int index[3];
    for(int i = 0; i < 3; i++){
        index[i] = i+1;
        pthread_create(&pid[i], NULL, (void*)thread, &index[i]);
    }
    int *status = malloc(4);
    for(int i = 0; i < 3; i++){
        pthread_join(pid[i], &status);
    }
}
