//
// Created by michail on 11.07.18.
//

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_MSG_SIZE 512
#define MAX_CLIENTS  10

//reader thread depends on value MAX_CLIENTS, don't change it, if you not sure

key_t key, sem_key;
int id, sem_id;
int myId;
char *msg;
char *public_buff;
char *nickname;

void drawer_thread()
{
    int isExit = 0;
    struct sembuf logic = {myId, -1, 0};
    while(!isExit){
        semop(sem_id, &logic, 1);
        printf("%s\n",public_buff);
    }
}

char *my_getline()
{
    char* buff = malloc(512 * sizeof(char));
    int i = 0;
    while(i < 255){
        char c;
        read(STDOUT_FILENO, &c, 1);
        buff[i] = c;
        if (buff[i] == '\n') {
            buff[i + 1] = 0;
            break;
        }else{
            i++;
        }
    }
    return buff;
}

void reader_thread()
{
    int isExit = 0;
    char buff[1024];
    struct sembuf logic[2] = {{1, 0, 0}, {1, 1, 0}};
    while (!isExit){
        msg = my_getline();
        if (!strcmp(msg, "quit\n")) isExit = 1;
        semop(sem_id, logic, 2);
        strcpy(buff, "");
        strcat(buff, nickname);
        strcat(buff, msg);
        buff[strlen(buff)] = 0;
        strcpy(public_buff, buff);
    }
}

int main()
{
    nickname = my_getline();
    int nickLength = strlen(nickname);
    nickname[nickLength - 1] = ':';
    nickname[nickLength] = ' ';
    nickname[nickLength + 1] = 0;
    key = ftok("/home/michail/summerSchool/les1/Makefile", 'B');
    sem_key = ftok("/home/michail/summerSchool/les1/Makefile", 'A');
    id = shmget(key, MAX_MSG_SIZE, 0);
    //semaphore 0 is a counter to clients
    sem_id = semget(sem_key, 1 + MAX_CLIENTS, 0);
    struct sembuf logic = {0, 1, 0};
    semop(sem_id, &logic, 1);
    myId = semctl(sem_id, 0, GETVAL);
    printf("My id = %i",myId);
    public_buff = shmat(id, 0, 0);

    pthread_t drawer_pid;
    pthread_t reader_pid;
    pthread_create(&drawer_pid, NULL, (void*)drawer_thread, NULL);
    pthread_create(&reader_pid, NULL, (void*)reader_thread, NULL);
    pthread_join(reader_pid, NULL);
    free(msg);
    free(nickname);
}