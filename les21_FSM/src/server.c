//
// Created by michail on 18.07.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#define CREATE_HANDLERS 1
#define CREATE_QUEUE    2
#define WAIT_FOR_CLIENT 3
#define QUEUE_CLIENT   4
#define TALKING         5
#define ERROR -1

int isExit = 0;

struct msgbuf{
    long priority;
    int fd;
    struct sockaddr_in address;
};

void waitClient(key_t key, int msgQueue)
{
    struct msgbuf buff;
    char buffer[256] = "Hello, my dear client. Are you ok?\n";
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int state = WAIT_FOR_CLIENT;
    int len;
    while(!isExit) {
        switch(state){
            case WAIT_FOR_CLIENT:
                printf("%i entered WAIT_FOR_CLIENT state\n", getpid());
                msgrcv(msgQueue, &buff, sizeof(buff) - sizeof(long), 1, 0);
                if (buff.fd == -1 || errno != 0){
                    isExit = 1;
                    break;
                }
                else state = TALKING;
                printf("pid %i socket %i (%s)\n", getpid(), sock, strerror(errno));
                //Send hello message
                strcpy(buffer, "Hello, my dear client. Are you ok?\n");
                len = sizeof(buff.address);
                sendto(sock, buffer, 256, 0, (struct sockaddr*)&buff.address, (socklen_t)len);
                break;
            case TALKING:
                printf("%i entered TALKING state\n", getpid());
                recvfrom(sock, buffer, 256, 0, (struct sockaddr*)&buff.address, (socklen_t*)&len);
                printf("%i received %s\n",getpid(), buffer);
                if (strlen(buffer) <= 3) {
                    state = ERROR;
                    break;
                }
                buffer[0] = 'H';
                buffer[1] = 'I';
                buffer[2] = '_';
                sendto(sock, buffer, 256, 0, (struct sockaddr*)&buff.address, (socklen_t)len);
                break;
            case ERROR:
                printf("%i entered ERROR state\n", getpid());
                strcpy(buffer, "Sorry, your string was too small... Shutting down the connection.");
                sendto(sock, buffer, 256, 0, (struct sockaddr*)&buff.address, (socklen_t)len);
                state = WAIT_FOR_CLIENT;
        }
    }
    if (state == WAIT_FOR_CLIENT) exit(0);
    printf("CRASHED child %i! in %i state \n", getpid(), state);
    exit(15);
}

void handler()
{
    isExit = 1;
}

void main_handler()
{
    isExit = 1;
    for(int i = 0; i < 10; i++){
        printf("Waiting for process %i to shutdown ", i);
        wait(NULL);
        printf("DONE\n");
    }
    exit(0);
}

int main()
{
    signal(SIGINT, (void*)handler);
    int state;
    key_t key;
    char buffer[256];
    int  len, sock, msgQueue;

    struct sockaddr_in address;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(15555);
    address.sin_family = AF_INET;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(sock, (struct sockaddr*)&address, sizeof(address));

    struct msgbuf msgbuf;
    state = CREATE_QUEUE;
    while(!isExit){
        printf("state = %i   ", state);
        switch(state){
            case CREATE_QUEUE:
                key = ftok("/home/michail/summerSchool/Makefile", 'A');
                msgQueue = msgget(key, IPC_CREAT | 0664);
                if (errno != 0){
                    state = ERROR;
                    break;
                }
                printf("Queue created\n");
                state = CREATE_HANDLERS;
                break;
            case CREATE_HANDLERS:
                for(int i = 0; i < 10; i++) {
                    if (fork() == 0) {
                        waitClient(key, msgQueue);
                        exit(-25);
                    }
                }
                signal(SIGINT, (void*)main_handler);
                if (errno != 0){
                    state = ERROR;
                    break;
                }
                printf("Handlers created (my pid = %i)\n", getpid());
                state = WAIT_FOR_CLIENT;
                break;
            case WAIT_FOR_CLIENT:
                len = sizeof(address);
                recvfrom(sock, buffer, 256, 0, (struct sockaddr*)&address, (socklen_t*)&len);
                if (errno != 0){
                    state = ERROR;
                    break;
                }
                state = QUEUE_CLIENT;
                break;
            case QUEUE_CLIENT:
                msgbuf.priority = 1;
                msgbuf.address = address;
                msgsnd(msgQueue, &msgbuf, sizeof(msgbuf) - sizeof(long), 0);
                state = WAIT_FOR_CLIENT;
                break;
            default:
                isExit = 1;
                break;
        }
    }
    printf("Main process finished with %i code on %i state", errno, state);
}