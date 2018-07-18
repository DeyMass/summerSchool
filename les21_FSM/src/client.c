//
// Created by michail on 18.07.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/msg.h>

#define CREATE_HANDLERS 1
#define CREATE_QUEUE    2
#define WAIT_FOR_CLIENT 3
#define QUEUE_CLIENT   4
#define TALKING         5
#define ERROR -1

struct msgbuf{
    long priority;
    int fd;
    struct sockaddr_in address;
};

int main()
{
    int state;
    key_t key;
    char buffer[256];
    int len, sock;
    key = ftok("/home/michail/summerSchool/Makefile", 'A');
    int message = msgget(key, 0);
    struct sockaddr_in address;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(15555);
    address.sin_family = AF_INET;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(sock, &address, sizeof(address));
    while(1){
        switch(state){
            case CREATE_QUEUE:
                struct msgbuf message;
                message.priority = 1L;
                message.address = address;
                message.fd = sock;
                msgsnd(message, )
            case CREATE_HANDLERS:
            case WAIT_FOR_CLIENT:
                len = sizeof(address);
                recvfrom(sock, buffer, 256, 0, &address, &len);

                state = QUEUE_CLIENT;
                break;
            case TALKING:

            default:
                printf("ERROR OCCURRED ON %i STATE", state);
                break;
        }
    }

}