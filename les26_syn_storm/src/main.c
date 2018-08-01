//
// Created by michail on 26.07.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define BEST_PORT_EVAR 1555

struct __attribute__((packed)) packet {
    unsigned int version:4;
    unsigned int IHL:4;
    unsigned int DSCP:6;
    unsigned int ECN:2;
    unsigned int payload:16;
    //32
    unsigned int id:16;
    unsigned int flags:3;
    unsigned int offt:13;
    //64
    unsigned int ttl:8;
    unsigned int proto:8;
    unsigned int csum:16;
    //96
    unsigned int saddr:32;
    unsigned int daddr:32;
    //160
    //unsigned int params:320;

    //TCPHEADER
    unsigned int sport:16;
    unsigned int dport:16;
    //32
    unsigned int snum:32;
    unsigned int anum:32;
    //96
    unsigned int dataOfft:4;
    unsigned int reserved:3;
    unsigned int flag:9;
    //112
    unsigned int winsz:16;
    unsigned int chsum:16;
    unsigned int urg:16;
    //140
    unsigned char opts[20];
    //160
    //struct tcphdr tcpheader;
//    char buffer[256];
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

int main()
{
    srand(time(NULL));
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    struct packet pck;
    bzero(&pck, sizeof(pck));
    unsigned short int *csum;
    char *cp;
    unsigned short int *sp;
    int *ip;
    int *seqNum;
    cp = &pck;
    cp[0] = 0x45;//version 4, Header Length 20 b
    cp[1] = 0;//DSCP + ECN -> 0
    sp = &cp[2];
    sp[0] = htons(60);
    sp[1] = htons(1);
    cp = &sp[2];
    cp[0] = 0x40;//flag + offset is set to 010 and 0
    cp[1] = 0;// flags + offset is set to 010 and 0
    cp[2] = 64; //ttl = 1
    cp[3] = 6; //protocol = tcp
    sp = &cp[4];
    csum = sp;
    ip = &sp[1];
    ip[0] = inet_addr("192.168.0.60");//src address
    ip[1] = inet_addr("192.168.0.30");//dest address
    csum[0] = checksum(&pck, &ip[2]);
    sp = &ip[2];
    sp[0] = htons(1333);//0-15 bytes SOURCE PORT
    sp[1] = htons(1555);//16-31 bytes DEST PORT
    ip = &sp[2];
    seqNum = &sp[2];
    ip[0] = htonl(10);//32-63 bytes SEQ NUM
    ip[1] = htonl(0);//64-93 bytes SEQACK NUM
    cp = &ip[2];
    //0000011 -> 1100000
    cp[0] = 5 << 5;//96-103 bytes DATA OFF_T + RESERVED + NS_FLAG
    cp[1] = 1 << 1;//104-111 bytes (6 byte = SYN)
    sp = &cp[2];
    sp[0] = htons(2048);//112-127 bytes WIN SIZE
    sp[1] = 0; //128-144 bytes CHECKSUM
    sp[2] = 0; //145-159 bytes URG PTR
    //OPT MAX SEG
    cp = &sp[3];
    cp[0] = 0x2;
    cp[1] = 0x4;
    sp = &cp[2];
    sp[0] = htons(1460);
    //OPT SACK
    cp = &sp[1];
    cp[0] = 0x4;
    cp[1] = 0x2;
    //OPT TIMESTAMP
    cp[2] = 0x8;
    cp[3] = 0xA;
    ip = &cp[4];
    ip[0] = htonl((unsigned int)rand());
    ip[1] = 0;
    //OPT NO OP
    cp = &ip[2];
    cp[0] = 0x1;
    //OPT WIN SCALE
    cp[1] = 0x3;
    cp[2] = 0x3;
    cp[3] = 0x7;

    printf("Size of pck = %i\n",sizeof(pck));

    //----------------------------------------------
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("192.168.0.30");
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(BEST_PORT_EVAR);
    socklen_t len = sizeof(addr);
    //----------------------------------------------

    if(errno != 0){
        printf("Error %s\n",strerror(errno));
    }
    int val = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val));
//    while(1) {
//        for (int i = 0; i < 65535; i++) {
//            *seqNum = htonl(i);
    sendto(sock, &pck, sizeof(pck), 0, (struct sockaddr *) &addr, len);
//        }
//    }
    if(errno != 0){
        printf("Error %s\n",strerror(errno));
    }

}

