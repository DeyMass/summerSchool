//
// Created by michail on 16.07.18.
//

#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <stdio.h>

int main()
{
    struct sockaddr_in address;
    struct sockaddr_in client;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(1555);

    //int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        printf("socket = %i\n", sock);
        printf("error occured: %s\n", strerror(errno));
    }
    bind(sock, &address, sizeof(address));
    listen(sock, 5);

    char buff[25];
    int addrlen = sizeof(address);
    int subsocket = accept(sock, &address, &addrlen);
    for(int i = 0; i < 26; i++) {
        recv(subsocket, buff, 25, 0);
        //recvfrom(sock, buff, 25, 0, &address, &addrlen);

        buff[1] = 'A' + i;

        send(subsocket, buff, 25, 0);
        //sendto(sock, buff, 25, 0, &address, sizeof(address);
        printf("%s\n", buff);
    }
}
