//
// Created by michail on 30.07.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <net/if.h>

struct sockaddr_ll {
        unsigned short  sll_family;    /* Всегда AF_PACKET */
        unsigned short  sll_protocol;  /* Протокол физического уровня */
        int             sll_ifindex;   /* Hомер интерфейса */
        unsigned short  sll_hatype;    /* Тип заголовка */
        unsigned char   sll_pkttype;   /* Тип пакета */
        unsigned char   sll_halen;     /* Длина адреса */
        unsigned char   sll_addr[8];   /* Адрес физического уровня */
};

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

    //UDPHEADER
    unsigned int sport:16;
    unsigned int dport:16;
    //32
    unsigned int length:16;
    unsigned int chsum:16;
    char buffer[256];
};

int main()
{

    struct packet pck;
    bzero(&pck, sizeof(pck));
    char *cp;
    unsigned short int *sp;
    int *ip;
    //IP HEADER
    cp = &pck;
    cp[0] = 0x45;//version 4, Header Length 20 b
    cp[1] = 0;//DSCP + ECN -> 0
    sp = &cp[2];
    sp[0] = htons(sizeof(pck));
    sp[1] = htons(1);
    cp = &sp[2];
    cp[0] = 0x40;//flag + offset is set to 010 and 0
    cp[1] = 0;// flags + offset is set to 010 and 0
    cp[2] = 5; //ttl = 1
    cp[3] = 17;//UDP //6; //protocol = tcp
    sp = &cp[4];
    sp[0] = 0;//checksum();// checksum
    ip = &sp[1];
    ip[0] = inet_addr("192.168.0.60");//src address
    ip[1] = inet_addr("192.168.0.30");//dest address
    sp = &ip[2];
    //UDP HEADER
    sp[0] = htons(1333);//0-15 bytes SOURCE PORT
    sp[1] = htons(1555);//16-31 bytes DEST PORT
    sp[2] = 256;
    sp[3] = 0;//checksum();
    strcpy(pck.buffer, "Hello");

    int sock = socket(AF_PACKET, SOCK_RAW, IPPROTO_TCP);
    struct sockaddr_ll addr;
    bzero(&addr, sizeof(addr));
    //me = 48:5d:60:08:5d:1f
//    addr.sll_addr[0] = 0x48;
//    addr.sll_addr[1] = 0x5d;
//    addr.sll_addr[2] = 0x60;
//    addr.sll_addr[3] = 0x08;
//    addr.sll_addr[4] = 0x5d;
//    addr.sll_addr[5] = 0x1f;
    //rpi = b8:27:eb:20:e4:2c
    addr.sll_addr[0] = 0xb8;
    addr.sll_addr[1] = 0x27;
    addr.sll_addr[2] = 0xeb;
    addr.sll_addr[3] = 0x20;
    addr.sll_addr[4] = 0xe4;
    addr.sll_addr[5] = 0x2c;
    addr.sll_ifindex = if_nametoindex("enp5s0f5");
    addr.sll_family = AF_PACKET;
    addr.sll_halen = 6;
    sendto(sock, &pck, sizeof(pck), 0, &addr, sizeof(addr));
    if (errno != 0){
        printf("Error %s\n", strerror(errno));
    }

}