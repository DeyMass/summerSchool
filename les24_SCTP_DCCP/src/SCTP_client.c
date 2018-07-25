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
    struct sctp_event_subscribe event;
    memset(&event, 0, sizeof(event));
    event.sctp_data_io_event = 1;
    char buffer[256];
    //----------------------------------------------
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(BEST_PORT_EVAR);
    //----------------------------------------------
    struct sctp_sndrcvinfo sInfo;
    socklen_t len = sizeof(addr);

    setsockopt(sock, SOL_SCTP, SCTP_EVENTS, &event, sizeof(event));
    connect(sock, &addr, sizeof(addr));
    if (errno != 0){
        printf("ERROR: %s\n",strerror(errno));
    }
    sInfo.sinfo_stream = 15;
    for(int i = 0; i < 15; i++){

        sctp_recvmsg(sock, buffer, 256, NULL, 0, &sInfo, 0);
        printf("received: %s (thr %i)\n",buffer, sInfo.sinfo_stream);
        buffer[0] = 'K';
        buffer[1] = 'Z';
        sctp_send(sock, buffer, 256, &sInfo, 0);
        sleep(1);
    }
}