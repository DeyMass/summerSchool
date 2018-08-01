//
// Created by michail on 31.07.18.
//

#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct __attribute((packed)) packet{
    struct icmphdr icmp;
};
struct __attribute((packed)) rcvpacket{
    struct iphdr ip;
    struct icmphdr icmp;
};

unsigned short int checksum(unsigned short int* startPoint, unsigned short int * endPoint)
{
    int i = 0;
    unsigned int result = 0;
    unsigned short int *tickPoint = startPoint;
    while(&tickPoint[i] != endPoint){
        printf("%x + %x", result, tickPoint[i]);
        result =(unsigned short int) result + tickPoint[i];
        printf(" = %x\n", result);
        if (result > 0xFFFF) {
            result -= 0x10000;
            result += 0x1;
        }
        i++;
    }
    printf("result = %x\n", ~((short int) result));
    return ~((short int)result);
}

int main(int argc, char* argv[])
{
    if (argc == 1){
        printf("No address is given");
        exit(-15);
    }
    printf("Ping to %s\n",argv[argc-1]);

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct packet pck;

    //----------------------------------------------
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(argv[argc-1]);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(1555);
    socklen_t len = sizeof(addr);
    //----------------------------------------------

    pck.icmp.type = 8;
    pck.icmp.code = 0;
    printf("--------------------------------");
    pck.icmp.checksum = htons(0xf6ff);//checksum(&pck, &pck + sizeof(pck));
    pck.icmp.checksum = checksum(&pck.icmp, &pck.icmp.un);
    printf("--------------------------------\n");
    sendto(sock, &pck, sizeof(pck), 0, (struct sockaddr*)&addr, len);
    struct rcvpacket rcv;
    char buffer[256];
    recvfrom(sock, &rcv, sizeof(rcv), 0, (struct sockaddr *) &addr, &len);
    sprintf(buffer, "Res code = %i (res type = %i)\n", rcv.icmp.code, rcv.icmp.type);
    write(STDOUT_FILENO, buffer, strlen(buffer));
    switch(rcv.icmp.type){
        case 0:
            sprintf(buffer, "Ping successfull\n", rcv.icmp.code, rcv.icmp.type);
            write(STDOUT_FILENO, buffer, strlen(buffer));
            break;
    }
}