//
// Created by michail on 16.07.18.
//

#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <stdio.h>

int main()
{
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(1555);

    //int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("socket = %i\n",sock);
    printf("errno: %s\n", strerror(errno));
    char buff[25] = "Hello world :3";
    int addrlen = sizeof(address);
    int newsock;
    if (connect(sock, &address, sizeof(address)) == -1) {
        printf("Can't connect\n");
        printf("errno: %s\n", strerror(errno));
        exit(1);
    }
    while(1) {
        send(sock, buff, 25, 0);
        //sendto(sock, buff, 25, 0, &address, sizeof(address));
        recv(sock, buff, 25, 0);
        //if (buff[1] == 'Z') break;
        //recvfrom(sock, buff, 25, 0, &address, &addrlen);
        printf("%s\n", buff);
    }
}