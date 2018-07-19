//
// Created by michail on 17.07.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

void udpClient()
{
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(25555);
    addr.sin_family = AF_INET;
    int connection;
    connection = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    char buffer[256];
    int len = sizeof(addr);
    while(1) {
        scanf("%s", buffer);
        if (!strcmp(buffer, "quit")) break;
        sendto(connection, buffer, 256, 0, &addr, len);
        recvfrom(connection, buffer, 256, 0, &addr, &len);
        printf("received %s\n",buffer);
    }
}

void tcpClient()
{
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(15555);
    addr.sin_family = AF_INET;
    int connection;
    connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int len = sizeof(addr);
    connect(connection, &addr, len);
    if(errno != 0){
        printf("error %s\n", strerror(errno));
        return;
    }
    while(1) {
        scanf("%s", buffer);
        if (!strcmp(buffer, "quit")) break;
        send(connection, buffer, 256, 0);
        recv(connection, buffer, 256, 0);
        printf("received %s\n",buffer);
    }
}

int main(int argc, char* argv[])
{
    if (!strcmp(argv[argc-1], "udp")) {
        printf("started udp client:\n");
        udpClient();
    }else{
        printf("started tcp client:\n");
        tcpClient();
    }
}
