//
// Created by michail on 19.07.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <wchar.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <malloc.h>

#define UDP_WAITING poll_t[1].revents
#define TCP_WAITING poll_t[0].revents


int max(int a, int b){
    return a > b ? a : b;
}

void server_on_epoll()
{
    int tcpSock;
    int udpSock;
    char buffer[256];
    struct sockaddr_in tcpAddr;
    struct sockaddr_in udpAddr;
    bzero(&tcpAddr, sizeof(tcpAddr));
    bzero(&udpAddr, sizeof(udpAddr));
    tcpAddr.sin_port = htons(15555);
    tcpAddr.sin_family = AF_INET;
    tcpAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    udpAddr.sin_port = htons(25555);
    udpAddr.sin_family = AF_INET;
    udpAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    udpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    tcpSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(tcpSock, (void*)&tcpAddr, sizeof(tcpAddr));
    listen(tcpSock, 10);
    bind(udpSock, (void*)&udpAddr, sizeof(udpAddr));

    int epfd = epoll_create(2);

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd  = tcpSock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, tcpSock, &event);

    struct epoll_event event1;
    event1.events = EPOLLIN;
    event1.data.fd  = udpSock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, udpSock, &event1);

    struct epoll_event events[2];

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    for(int i = 0; i < 10; i++) {
        if(errno != 0){
            printf("%s\n",strerror(errno));
            return;
        }

        int res = epoll_wait(epfd, events, 2, -1);
        printf("%i (%i %i) (udp = %i tcp = %i)\n", res, events[0].data.fd, events[1].data.fd, udpSock, tcpSock);
        if(events[0].data.fd == udpSock){
            recvfrom(udpSock, buffer, 256, 0, (void*)&addr, &len);
            printf("received via udp %s\n", buffer);
            buffer[0] = 'H';
            buffer[1] = 'O';
            buffer[2] = 'L';
            buffer[3] = 'A';
            buffer[4] = '_';
            sendto(udpSock, buffer, 256, 0, (void*)&addr, len);
        }
        if(events[0].data.fd == tcpSock){
            printf("receiving");
            int newSock = accept(tcpSock, (void*)&addr, &len);
            recv(newSock, buffer, 256, 0);
            printf("received via tcp %s\n", buffer);
            buffer[0] = 'H';
            buffer[1] = 'O';
            buffer[2] = 'L';
            buffer[3] = 'A';
            buffer[4] = '_';
            send(newSock, buffer, 256, 0);
            close(newSock);
        }
    }
}

void server_on_poll()
{
    int tcpSock;
    int udpSock;
    char buffer[256];
    struct sockaddr_in tcpAddr;
    struct sockaddr_in udpAddr;
    bzero(&tcpAddr, sizeof(tcpAddr));
    bzero(&udpAddr, sizeof(udpAddr));
    tcpAddr.sin_port = htons(15555);
    tcpAddr.sin_family = AF_INET;
    tcpAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    udpAddr.sin_port = htons(25555);
    udpAddr.sin_family = AF_INET;
    udpAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    udpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    tcpSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(tcpSock, (void*)&tcpAddr, sizeof(tcpAddr));
    listen(tcpSock, 10);
    bind(udpSock, (void*)&udpAddr, sizeof(udpAddr));
    struct pollfd poll_t[2];
    poll_t[0].fd = tcpSock;
    poll_t[0].events = POLLIN;
    poll_t[0].revents = 0;

    poll_t[1].fd = udpSock;
    poll_t[1].events = POLLIN;
    poll_t[1].revents = 0;

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    for(int i = 0; i < 10; i++) {
        int res = poll(poll_t, 2, -1);
        printf("%i (%i %i)\n", res, poll_t[0].revents, poll_t[1].revents);
        if(UDP_WAITING){
            recvfrom(udpSock, buffer, 256, 0, (void*)&addr, &len);
            printf("received via udp %s\n", buffer);
            buffer[0] = 'H';
            buffer[1] = 'O';
            buffer[2] = 'L';
            buffer[3] = 'A';
            buffer[4] = '_';
            sendto(udpSock, buffer, 256, 0, (void*)&addr, len);
        }
        if(TCP_WAITING){
            printf("receiving");
            int newSock = accept(tcpSock, (void*)&addr, &len);
            recv(newSock, buffer, 256, 0);
            printf("received via tcp %s\n", buffer);
            buffer[0] = 'H';
            buffer[1] = 'O';
            buffer[2] = 'L';
            buffer[3] = 'A';
            buffer[4] = '_';
            send(newSock, buffer, 256, 0);
            close(newSock);
        }
    }
}

void server_on_select()
{
    fd_set mask;
    int tcpSock;
    int udpSock;
    char buffer[256];
    struct sockaddr_in tcpAddr;
    struct sockaddr_in udpAddr;
    bzero(&tcpAddr, sizeof(tcpAddr));
    bzero(&udpAddr, sizeof(udpAddr));
    tcpAddr.sin_port = htons(15555);
    tcpAddr.sin_family = AF_INET;
    tcpAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    udpAddr.sin_port = htons(25555);
    udpAddr.sin_family = AF_INET;
    udpAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    udpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    tcpSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(tcpSock, (void*)&tcpAddr, sizeof(tcpAddr));
    listen(tcpSock, 10);
    bind(udpSock, (void*)&udpAddr, sizeof(udpAddr));

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    for(int i = 0; i < 10; i++) {
        FD_ZERO(&mask);
        FD_SET(0, &mask);
        FD_SET(tcpSock, &mask);
        FD_SET(udpSock, &mask);
        int res = select(max(udpSock, tcpSock) + 1, &mask, NULL, NULL, NULL);
        printf("%i (%i %i)\n", res, FD_ISSET(udpSock, &mask), FD_ISSET(tcpSock, &mask));
        if(FD_ISSET(udpSock, &mask)){
            recvfrom(udpSock, buffer, 256, 0, (void*)&addr, &len);
            printf("received via udp %s\n", buffer);
            buffer[0] = 'H';
            buffer[1] = 'O';
            buffer[2] = 'L';
            buffer[3] = 'A';
            buffer[4] = '_';
            sendto(udpSock, buffer, 256, 0, (void*)&addr, len);
        }
        if(FD_ISSET(tcpSock, &mask)){
            int newSock = accept(tcpSock, (void*)&addr, &len);
            recv(newSock, buffer, 256, 0);
            printf("received via tcp %s\n", buffer);
            buffer[0] = 'H';
            buffer[1] = 'O';
            buffer[2] = 'L';
            buffer[3] = 'A';
            buffer[4] = '_';
            send(newSock, buffer, 256, 0);
            close(newSock);
        }
        if(FD_ISSET(STDIN_FILENO, &mask)){
            read(STDIN_FILENO, buffer, 1);
        }
    }
}

int main(int argc, char* argv[])
{
    if (!strcmp(argv[argc-1], "epoll")) {
        server_on_epoll();
    }else
    if (!strcmp(argv[argc-1], "poll")) {
        server_on_poll();
    }else
    if (!strcmp(argv[argc-1], "select")) {
        server_on_select();
    }else puts("Select poll|epoll|select\n");

}