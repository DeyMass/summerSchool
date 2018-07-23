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
    addr.sin_addr.s_addr = inet_addr("224.0.0.1");
    addr.sin_port = htons(BEST_PORT_EVAR);

    struct ip_mreq R;
    bzero(&R, sizeof(R));
    R.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &R, sizeof(R));
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    socklen_t len = sizeof(addr);
    char buffer[256] = "Hello my clients!";
    if (errno != 0){
        printf("Error %s",strerror(errno));
        return -1;
    }
    for(int i = 0; i < 5; i++) {
        recvfrom(fd, buffer, 256, 0, (struct sockaddr*)&addr, &len);
        sleep(1);
    }

}