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
#include <linux/if_ether.h>
#include <errno.h>
#include <poll.h>

struct __attribute((packed)) packet{
    struct iphdr ip;
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
        result =(unsigned short int) result + tickPoint[i];
        if (result > 0xFFFF) {
            result -= 0x10000;
            result += 0x1;
        }
        i++;
    }
    return ~((short int)result);
}

int main(int argc, char *argv[])
{
    if (argc == 1){
        printf("No address is given");
        exit(-15);
    }
    printf("Trace route to %s\n",argv[argc-1]);

    unsigned short int *ipHeaderStart;
    unsigned char *ttl;
    unsigned char *cp;
    unsigned short int *sp;
    unsigned int *ip;
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    int recsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct packet pck;
    bzero(&pck, sizeof(pck));
    //IP HEADER
    cp = &pck;
    ipHeaderStart = cp;
    cp[0] = 0x45;//version 4, Header Length 20 b
    cp[1] = 0;//DSCP + ECN -> 0
    sp = &cp[2];
    sp[0] = htons(20 + 256 + 8); //PACKET LEN
    //48 - SIZE OF IP HEADER 8 - SIZE OF UDP HEADER 256 - BUFFER
    sp[1] = htons(4267);//Identification
    cp = &sp[2];
    cp[0] = 0x40;//flag + offset is set to 010 and 0
    cp[1] = 0;// flags + offset is set to 010 and 0
    cp[2] = 1; //ttl = 1
    ttl = &cp[2];
    cp[3] = 1;//ICMP //17;//UDP //6; //protocol = tcp
    sp = &cp[4];
    unsigned short int *csum = sp;
    sp[0] = 0;//checksum();// checksum
    ip = &sp[1];
    ip[0] = inet_addr("192.168.0.60");//src address
    ip[1] = inet_addr(argv[argc-1]);//dest address
    //Calc checksum!
    csum[0] = checksum(ipHeaderStart, (unsigned short int*)&ip[2]);

    int val = 1;
    //dup2(recsock, sock);
    setsockopt(sock, SOL_IP, IP_HDRINCL, &val, sizeof(val));
    //----------------------------------------------
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(argv[argc-1]);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(1555);
    socklen_t len = sizeof(addr);
    //----------------------------------------------
    struct sockaddr_in raddr;
    raddr.sin_addr.s_addr = INADDR_ANY;
    raddr.sin_family      = AF_PACKET;
    socklen_t rlen = sizeof(raddr);
    //----------------------------------------------
    if(errno != 0){
        printf("Error %s\n", strerror(errno));
        return 0;
    }
    pck.icmp.type = 8;
    pck.icmp.code = 0;
    printf("--------------------------------");
    pck.icmp.checksum = checksum(&pck.icmp, &pck.icmp.un);
    //pck.icmp.checksum = htons(0xF7FF);//checksum(&pck.icmp, &pck.icmp + sizeof(struct icmphdr));//checksum(&pck, &pck + sizeof(pck));

    printf("--------------------------------\n");
    struct rcvpacket rcv;
    rcv.icmp.type = 11;
    int  pollRes;
    int  count = 1;
    char buffer[256];
    unsigned char *bytes;

    struct pollfd fd;
    fd.fd = sock;
    fd.events = POLLIN;
    while(rcv.icmp.type == 11) {
        sendto(sock, &pck, sizeof(pck), 0, (struct sockaddr *) &addr, len);
        //timeout (poll)
        pollRes = poll(&sock, 1, 5000);
        if(pollRes != 0) {
            recvfrom(sock, &rcv, sizeof(pck), 0, (struct sockaddr *) &addr, &rlen);
            bytes = &rcv.ip.saddr;
            int A, B, C, D;
            A = bytes[3] & 0xFF;
            B = bytes[2] & 0xFF;
            C = bytes[1] & 0xFF;
            D = bytes[0] & 0xFF;
            switch(rcv.icmp.type) {
                case 11:
                    sprintf(buffer, "%i: %i.%i.%i.%i\n", count, A, B, C, D);
                    write(STDOUT_FILENO, buffer, strlen(buffer));
                    break;
                case 3:
                    sprintf(buffer, "%i: destination unreachable\n", count, A, B, C, D);
                    write(STDOUT_FILENO, buffer, strlen(buffer));
                    break;
                case 0:
                    sprintf(buffer, "%i: REACHED DESTINATION %i.%i.%i.%i\n", count, A, B, C, D);
                    write(STDOUT_FILENO, buffer, strlen(buffer));
                    break;
            }
        }else{
            sprintf(buffer, "%i: UNKNOWN\n", count);
            write(STDOUT_FILENO, buffer, strlen(buffer));
        }
        pck.ip.ttl++;
        count++;
    }
    sprintf(buffer, "Res code = %i (res type = %i)\n", rcv.icmp.code, rcv.icmp.type);
    write(STDOUT_FILENO, buffer, strlen(buffer));
}