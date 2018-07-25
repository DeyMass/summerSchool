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
    char buffer[256];
    strcpy(buffer, "Hello, client");
    //----------------------------------------------
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(BEST_PORT_EVAR);
    //----------------------------------------------
    bind(sock, &addr, sizeof(addr));
    listen(sock, 10);
    struct sctp_sndrcvinfo sInfo;
    socklen_t len = sizeof(addr);

    int recSock = accept(sock, &addr, &len);
    if (errno != 0){
        printf("ERROR: %s\n",strerror(errno));
    }
    bzero(&sInfo, sizeof(sInfo));
    for(int i = 0; i < 15; i++) {
        sctp_send(recSock, buffer, 256, &sInfo, 0);
        sctp_recvmsg(recSock, buffer, 256, &addr, &len, &sInfo, 0);
        sleep(1);
    }

}