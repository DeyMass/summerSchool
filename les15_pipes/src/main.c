//
// Created by michail on 09.07.18.
//

#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <sys/wait.h>

int main(){
    int fd[2];// 0 - read, 1 - write
    int output;

    output = dup(STDOUT_FILENO);
    pipe(fd);
    int input;
    char buff[1024];
    ssize_t readed;
    readed = read(0, buff, 256);
    write(output, buff, readed);
    int pid = fork();
    switch(pid){
        case 0:
            input = dup(STDIN_FILENO);
            readed = 1;
            close(fd[1]);
            while(readed > 0){
                readed = read(input, &buff, 1024);
                if(readed == 0) break;
                write(output, buff, readed);
                write(output, "\n", 1);
            }
            close(fd[0]);
            _exit(0);
        default:
            write(input, "Hello world", strlen("hello world"));
            close(input);
            wait(0);
            _exit(0);
    }
}