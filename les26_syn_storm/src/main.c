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

#define BEST_PORT_EVAR 7777

struct __attribute__((packed)) packet {
    unsigned int sport:16;
    unsigned int dport:16;
    //16
    unsigned int snum:32;
    unsigned int anum:32;
    //64
    unsigned int dataOfft:4;
    unsigned int reserved:3;
    unsigned int flags:9;
    //16
    unsigned int winsz:16;
    unsigned int csum:16;
    unsigned int urg:16;
    unsigned char opts[20];
    //struct tcphdr tcpheader;
//    char buffer[256];
};

int main()
{
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    struct packet pck;
    bzero(&pck, sizeof(pck));
    char *cp;
    unsigned short int *sp;
    int *ip;
    int *seqNum;
    sp = &pck;
    sp[0] = htons(1333);//0-15 bytes SOURCE PORT
    sp[1] = htons(1555);//16-31 bytes DEST PORT
    ip = &sp[2];
    seqNum = &sp[2];
    ip[0] = htonl(0);//32-63 bytes SEQ NUM
    ip[1] = htonl(0);//64-93 bytes SEQACK NUM
    cp = &ip[2];
    //0000011 -> 1100000
    cp[0] = 5 << 5;//96-103 bytes DATA OFF_T + RESERVED + NS_FLAG
    cp[1] = 1 << 1;//104-111 bytes (6 byte = SYN)
    sp = &cp[2];
    sp[0] = htons(29200);//112-127 bytes WIN SIZE
    sp[1] = htons(0x81d9); //128-144 bytes CHECKSUM
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
    ip[0] = htonl(987654321);
    ip[1] = 0;
    //OPT NO OP
    cp = &ip[2];
    cp[0] = 0x1;
    //OPT WIN SCALE
    cp[1] = 0x3;
    cp[2] = 0x3;
    cp[3] = 0x7;

    printf("Size of pck = %i\n",sizeof(pck));

    //pck.sport = htons(1555);
    //pck.dport = htons(1333);
    //pck.tcpheader.th_dport = htons(1555);
    //pck.tcpheader.th_sport = htons(1333);

    //pck.tcpheader.th_flags = 0x0 | (0x2 >> 4);

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
