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

int sem_id;

void printStatus() {
    while(1) {
        for (int sem_n = 0; sem_n < MAX_CLIENTS + 2; sem_n++) {
            printf("%i ", semctl(sem_id, sem_n, GETVAL));
        }
        printf("\n");
        sleep(1);
    }
}

char *msg;

int main()
{
    key_t key = ftok("/home/michail/summerSchool/les1/Makefile", 'B');
    key_t sem_key = ftok("/home/michail/summerSchool/les1/Makefile", 'A');
    int id = shmget(key, MAX_MSG_SIZE, IPC_CREAT | 0664);
    //semaphore 0 is counting clients
    //semaphore 1 is detecting a new input string
    sem_id = semget(sem_key, 2 + MAX_CLIENTS, IPC_CREAT | 0664);
    semctl(sem_id, 0, SETVAL, 2);
    semctl(sem_id, 1, SETVAL, 0);

    msg = shmat(id, 0, 0);
    struct sembuf logic[3] = {{1, -1, 0}, {1, 0, 0}, {1, 1, 0}};
    int isExit = 0;
    pthread_t pid;
    pthread_create(&pid, NULL, printStatus, NULL);
    while(!isExit){
        semop(sem_id, logic, 3);
        printf("message!\n");
        if (!strcmp(msg, "quit\n")) break;
        if (!strcmp(msg, "quit")) break;
        for (int i = 2; i < 2 + MAX_CLIENTS; i++){
            logic[0].sem_num = i;
            logic[0].sem_op = 1;
            semop(sem_id, logic, 1);
        }
        logic[0].sem_num = 1;
        logic[0].sem_op = -1;
        semop(sem_id, logic, 1);
    }
    shmdt(msg);
}