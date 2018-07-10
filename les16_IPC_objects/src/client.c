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
#include <asm/errno.h>

#define SERVICE_MESSAGE 1
#define CLIENT_MESSAGE  2
#define MIDDLE_MESSAGE  3

char *username;
key_t key;
int service_desc;
int middle_desc;
int client_desc;

struct msgbuf{
    long mtype;
    char username[32];
    char mtext[256];
};

int isExit = 0;

void screen_reader(void)
{
    char buff[256];
    struct msgbuf message;
    printf("user %s is set\n", username);
    strcpy(message.username, username);
    while(!isExit){
        int i = 0;
        while(i < 254){
            char c;
            read(STDOUT_FILENO, &c, 1);
            buff[i] = c;
            if (buff[i] == '\n') {
                break;
            }else{
                i++;
            }
        }
        buff[i+1] = 0;
        if (!strcmp(buff, "quit\n")){
            write(STDOUT_FILENO, "shutdown\n", strlen("shutdown\n"));
            isExit = 1;
            break;
        }
        strcpy(message.mtext, buff);
        message.mtype = MIDDLE_MESSAGE;
        msgsnd(middle_desc, &message, sizeof(message) - sizeof(long), 0);
        i = 0;
        buff[i] = 0;
    }
}

void screen_printer(void)
{
    struct msgbuf message;
    char buff[512];
    while(!isExit) {
        msgrcv(client_desc, &message, sizeof(message) - sizeof(long), CLIENT_MESSAGE, 0);
        sprintf(buff, "%s: %s\n", message.username, message.mtext);
        write(STDOUT_FILENO, buff, strlen(buff));
    }
}

char *my_getline()
{
    char* buff = malloc(256 * sizeof(char));
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

int main()
{
    while(1) {
        key = ftok("/home/michail/summerSchool/Makefile", 'A');
        service_desc = msgget(key, 0);
        key = ftok("/home/michail/summerSchool/Makefile", 'B');
        client_desc = msgget(key, 0);
        key = ftok("/home/michail/summerSchool/Makefile", 'C');
        middle_desc = msgget(key, 0);

        if (service_desc == ENOENT || client_desc == ENOENT || middle_desc == ENOENT) {
            write(STDOUT_FILENO, "error, waiting for server\n", strlen("error, waiting for server\n"));
        }else{
            break;
        }
    }
    write(STDOUT_FILENO, "enter username: ", strlen("enter username: "));
    username = my_getline();
    username[strlen(username)-1] = 0;// remove last '\n' symbol
    struct msgbuf message;
    message.mtext[0] = 0;
    message.mtype = SERVICE_MESSAGE;

    strcpy(message.username, username);

    msgsnd(service_desc, &message, sizeof(message) - sizeof(long), 0);

    pthread_t reader_pid;
    pthread_t receiver_pid;

    pthread_create(&reader_pid, 0, (void *) screen_reader, 0);
    pthread_create(&receiver_pid, 0, (void *) screen_printer, 0);

    pthread_join(receiver_pid, NULL);
    pthread_join(reader_pid, NULL);
    free(username);
}