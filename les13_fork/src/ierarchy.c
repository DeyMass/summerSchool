//
// Created by michail on 03.07.18.
//

#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"

int main()
{
    pid_t child1;
    pid_t child2;
    pid_t child3;
    pid_t child4;
    pid_t child5;
    child1 = fork();
    switch (child1){
        case 0:
            child3 = fork();
            switch(child3) {
                case 0:
                    printf("Hello world! my pid = %i, my parrent pid = %i\n", getpid(), getppid());
                    exit(0);
                default:
                    printf("Hello world! my pid = %i, my parrent pid = %i\n", getpid(), getppid());
            }
            break;
        default:
            child2 = fork();
            switch (child2){
                case 0:
                    child4 = fork();
                    switch (child4){
                        case 0:
                            printf("Hello world! my pid = %i, my parrent pid = %i\n", getpid(), getppid());
                            exit(0);
                        default:
                            printf("Hello world! my pid = %i, my parrent pid = %i\n", getpid(), getppid());
                    }
                    child5 = fork();
                    switch (child5){
                        case 0:
                            printf("Hello world! my pid = %i, my parrent pid = %i\n", getpid(), getppid());
                            exit(0);
                    }
                    break;
            }
    }
    int status;
    wait(&status);
    wait(&status);
    wait(&status);
    wait(&status);
    wait(&status);
    return 0;
}