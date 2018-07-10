//
// Created by michail on 09.07.18.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

char *my_getline()
{
    char* buff = malloc(256 * sizeof(char));
    int i = 0;
    while(i < 255){
        char c;
        read(STDOUT_FILENO, &c, 1);
        buff[i] = c;
        if (buff[i] == '\n') {
            buff[i + 1] = 0;
            break;
        }else{
            i++;
        }
    }
    return buff;
}


int main(int argc, char* argv[])
{
    printf("The best command interpretator in your life!\n");
    int fd[2];
    pipe(fd);

    int isExit = 0;
    char output[1024];
    char *buff;
    char command[512];
    char arg[512];
    int start = 0;
    int count_com = 0;
    while(!isExit){
        write(STDOUT_FILENO, "> ", 2);
        count_com++;
        buff = my_getline();
        for (int i = 0; i < strlen(buff); i++){
            if (buff[i] == '|' || buff[i] == '\n'){
                for(int j = start; j < i; j++){
                    command[j - start] = buff[j];
                }
                command[i] = 0;
                i++;
                while(buff[i] == ' ') i++;
                for(int j = strlen(command); j > 0; j--){
                    if (command[j] == ' ') command[j] = 0;
                    else break;
                }
                write(STDOUT_FILENO, "parse command: ", strlen("parse command: "));
                write(STDOUT_FILENO, command, strlen(command));
                write(STDOUT_FILENO, "\n", 1);
                start = i;
                if(fork() == 0){
                    dup2(fd[1], STDIN_FILENO);
                    char path[512] = "/bin/";
                    strcat(path, command);
                    write(STDOUT_FILENO, "started: ", strlen("started: "));
                    write(STDOUT_FILENO, path, strlen(path));
                    execl(path, path, NULL);
                    exit(0);
                }else{
                    dup2(STDOUT_FILENO, fd[1]);
                }
            }
        }
    }

}