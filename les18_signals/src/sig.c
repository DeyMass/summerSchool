//
// Created by michail on 12.07.18.
//

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void handle(int signum){
    write(STDOUT_FILENO, "handled\n", strlen("handled\n"));
}

int main()
{
    printf("my pid = %i", getpid());
    signal(SIGUSR1, handle);
    sigset_t set;
    sigset_t old;
    sigfillset(&set);
    struct sigaction sigact;
    sigact.sa_flags = 0;
    sigact.sa_handler = handle;
    sigact.sa_mask = set;
    sigaction(SIGUSR1, &sigact, NULL);
    sigprocmask(SIG_BLOCK, &set, &old);
    while (1) {
        write(STDOUT_FILENO, "i need some sleep\n", strlen("i need some sleep\n"));
        //sleep(2)
        pause();
    }
}