//
// Created by michail on 23.07.18.
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

#define BEST_PORT_EVAR 7777

int main()
{
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int val = 1;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.0.255");
    addr.sin_port = htons(BEST_PORT_EVAR);
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    socklen_t len = sizeof(addr);
    char buffer[256];
    if (errno != 0){
        printf("Error %s",strerror(errno));
        return -1;
    }
    for(int i = 0; i < 5; i++) {
        recvfrom(fd, buffer, 256, 0, (struct sockaddr*)&addr, &len);
        printf("received %s\n", buffer);
        sleep(1);
    }

}