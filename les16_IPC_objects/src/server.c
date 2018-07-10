//
// Created by michail on 10.07.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <string.h>

#define SERVICE_MESSAGE 1
#define CLIENT_MESSAGE  2
#define MIDDLE_MESSAGE  3

int clients_number = 0;
int service_desc;
int middle_desc;
int client_desc;
int isExit = 0;
key_t key;

struct msgbuf{
    long mtype;
    char username[32];
    char mtext[256];
};

void service_thread()
{
    struct msgbuf message;
    while(!isExit) {
        msgrcv(service_desc, &message, sizeof(message) - sizeof(long), SERVICE_MESSAGE, 0);
        char buff[512];
        sprintf(buff, "%s has joined the party!\n", message.username);
        clients_number++;
        write(STDOUT_FILENO, buff, strlen(buff));
    }
}

void redirecting_thread()
{
    struct msgbuf message;
    while(!isExit) {
        msgrcv(middle_desc, &message, sizeof(message) - sizeof(long), MIDDLE_MESSAGE, 0);
        char buff[512];
        sprintf(buff, "%s has written: %s!\n", message.username, message.mtext);
        write(STDOUT_FILENO, buff, strlen(buff));
        message.mtype = CLIENT_MESSAGE;
        for (int i = 0; i < clients_number; i++) {
            msgsnd(client_desc, &message, sizeof(message) - sizeof(long), 0);
        }
    }
}

int main()
{

    key = ftok("/home/michail/summerSchool/Makefile", 'A');
    service_desc = msgget(key, IPC_CREAT | 0664);
    key = ftok("/home/michail/summerSchool/Makefile", 'B');
    client_desc = msgget(key, IPC_CREAT | 0664);
    key = ftok("/home/michail/summerSchool/Makefile", 'C');
    middle_desc = msgget(key, IPC_CREAT | 0664);

    pthread_t service_pid;
    pthread_t client_pid;

    pthread_create(&service_pid, 0, (void*) service_thread, 0);
    pthread_create(&client_pid, 0, (void*) redirecting_thread, 0);
    char stop;
    write(STDOUT_FILENO, "Press enter to stop", strlen("Press enter to stop"));
    read(STDIN_FILENO, &stop, 1);
    isExit = 1;
    pthread_join(service_pid, NULL);
    pthread_join(client_pid, NULL);
}