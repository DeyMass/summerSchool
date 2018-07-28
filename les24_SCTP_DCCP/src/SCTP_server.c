//
// Created by michail on 24.07.18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BEST_PORT_EVAR 7777

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    char buffer[256] = "Hello, client!";
    //----------------------------------------------
    struct sockaddr_in addr, addr2;
    addr.sin_addr.s_addr = inet_addr("192.168.43.51");
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(BEST_PORT_EVAR);
    //----------------------------------------------
    addr2.sin_addr.s_addr = inet_addr("192.168.0.60");
    addr2.sin_family      = AF_INET;
    addr2.sin_port        = htons(BEST_PORT_EVAR);
    //----------------------------------------------
    bind(sock, &addr, sizeof(addr));
    sctp_bindx(sock, &addr2, 1, SCTP_BINDX_ADD_ADDR);
    listen(sock, 10);

    struct sctp_sndrcvinfo sInfo;
    socklen_t len = sizeof(addr);
    if (errno != 0){
        printf("ERROR: %s\n",strerror(errno));
        return -1;
    }

    int recSock = accept(sock, &addr, &len);
    if (errno != 0){
        printf("ERROR: %s\n",strerror(errno));
        return -1;
    }
    bzero(&sInfo, sizeof(sInfo));
    for(int i = 0; i < 15; i++) {
        sctp_send(recSock, buffer, 256, &sInfo, 0);
        //write(recSock, buffer, 256);
        sctp_recvmsg(recSock, buffer, 256, &addr, &len, &sInfo, 0);
        //read(recSock, buffer, 256);
        printf("Got %s\n", buffer);
        buffer[0] = 'A' + i;
        buffer[1] = 'Z' - i;
//        sctp_recvmsg(recSock, buffer, 256, &addr, &len, &sInfo, 0);
        sleep(1);
    }

}