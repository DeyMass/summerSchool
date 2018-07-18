//
// Created by michail on 17.07.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

void udpServer()
{
    int listenSock;
    struct sockaddr_in receiver;
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(15555);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");
    listenSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int len = sizeof(receiver);
    bind(listenSock, &receiver, len);
    char buffer[256];
    while(1){
        len = sizeof(receiver);
        recvfrom(listenSock, buffer, 256, 0, &receiver, &len);
        printf("A new client connected!\n");
        if (fork() == 0){
            listenSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            strcpy(buffer, "Hello, my dear client. Are you ok?\n");
            printf("sending %s \n", buffer);
            sendto(listenSock, buffer, 256, 0, &receiver, len);
            while(1){
                recvfrom(listenSock, buffer, 256, 0, &receiver, &len);
                printf("%i received %s\n", getpid(), buffer);
                buffer[0] = 'A';
                buffer[1] = 'Z';
                buffer[2] = '_';
                sendto(listenSock, buffer, 256, 0, &receiver, len);
            }
        }
    }
}

void tcpServer()
{
    int mainSock;
    struct sockaddr_in receiver;
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(15555);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");
    mainSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int len = sizeof(receiver);
    bind(mainSock, &receiver, len);
    listen(mainSock, 10);
    printf("Errno: %s",strerror(errno));
    int status;
    scanf("%i",&status);
    char buffer[256];
    while(1){
        len = sizeof(receiver);
        int subSock = accept(mainSock, &receiver, &len);
        printf("A new client connected!\n");
        if (fork() == 0){
            send(subSock, "You are connected!", strlen("You are connected!"), 0);
            while(1){
                recv(subSock, buffer, 256, 0);
                printf("%i received %s\n", getpid(), buffer);
                buffer[0] = 'A';
                buffer[1] = 'Z';
                buffer[2] = '_';
                send(subSock, buffer, 256, 0);
            }
        }
    }
}

int main()
{
    //udpServer();//
    tcpServer();
}