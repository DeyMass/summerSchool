//
// Created by michail on 25.07.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>

struct __attribute__((packed)) recvPacket{
    struct iphdr iphead;
    struct udphdr udphead;
    char buff[256];
};

struct packet{
    struct udphdr udpHeader;
    char buffer[256];
};

unsigned short int reflect(unsigned short int num){
    unsigned short int k = 0;
    k |= num & 0xFF;
    k = k << 8;
    k |= (num >> 8) & 0xFF;

    return k;
}

int main()
{
    struct udphdr header;
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    //----------------------------------------------
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(1555);
    //----------------------------------------------
    socklen_t len = sizeof(addr);
    struct packet pck;
    if (errno != 0){
        printf("Error %s\n", strerror(errno));
        return -1;
    }
    pck.udpHeader.check = 0;
    pck.udpHeader.source = htons(1222);
    pck.udpHeader.dest = htons(1555);
    pck.udpHeader.len = htons(256 + 8);
    strcpy(pck.buffer, "Hello, friend");
    sendto(sock, &pck, 256 + sizeof(struct udphdr), 0, &addr, len);

    struct recvPacket rcvpck;
    while(rcvpck.udphead.source != htons(1555)) {
        recvfrom(sock, &rcvpck, 256 + sizeof(struct udphdr) + sizeof(struct iphdr), 0, &addr, &len);
    }
    char buffer[512];
    sprintf(buffer, "received: %s\n", rcvpck.buff);
    write(STDOUT_FILENO, buffer, strlen(buffer));

}