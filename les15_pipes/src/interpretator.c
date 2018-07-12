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

void parse_args(char** arg, char* command)
{
    char *pointer;
    int flag = 0;
    for(int i = 0; i < strlen(command); i++){
        if (command[i] == 0) break;
        if (command[i] == ' ') {
            while(command[i] == ' ' && command[i] != 0) i++;
            pointer = &command[i];
            flag = 1;
            break;
        }
    }
    if(flag)
        strcpy(*arg, pointer);
    else
        strcpy(*arg, "");
}

void pipeline(char **com, int com_count, int *fd, int start) {
    pid_t pid;
    pipe(fd);
    pid = fork();

    int std_fd = dup(1);
    if (pid == 0) {

        if (com_count > 1) {
            pipeline(com, com_count - 1, fd, 0);
//wait(NULL);
            dup2(fd[0], 0);
        } else {
            close(fd[0]);
        }

        if (!start) {
            dup2(fd[1], 1);
        } else {
            close(fd[1]);
        }
        write(std_fd, "com: ", 5);
        write(std_fd, com[com_count - 1], strlen(com[com_count - 1]));
        write(std_fd, "\n", 1);
        system(com[com_count - 1]);
        exit(0);
    } else {
        wait(NULL);
        return;
    }

}

void recursive_call(char** programs, char** args, int count) {
    if (count <= 0) return;
    int fd[2];
    char* stdOrigin = malloc(9);
    stdOrigin[0] = '/';
    stdOrigin[1] = 'u';
    stdOrigin[2] = 's';
    stdOrigin[3] = 'r';
    stdOrigin[4] = '/';
    stdOrigin[5] = 'b';
    stdOrigin[6] = 'i';
    stdOrigin[7] = 'n';
    stdOrigin[8] = 0;
    for (int i = 0; i < count; i++) {
        pipe(fd);
        if (fork() == 0) {
            char path[1024];
            path[0] = 0;
//            strcpy(path, stdOrigin);
            strcat(path, programs[i]);
            printf("start %s",path);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            if (args[i][0] == ' ' || args[i][0] == 0) {
                execlp(path, path, NULL);
            } else
                execlp(path, path, args[i], NULL);
            exit(0);
        }
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
    }
    char outBuf;
    while (read(fd[0], &outBuf, 1)) write(STDOUT_FILENO, &outBuf, 1);
    close(fd[0]);
}

int main(int argc, char* argv[])
{
    printf("The best command interpretator in your life!\n");
    char **args = malloc(512 * sizeof(char*));
    char **commands = malloc(512 * sizeof(char*));
    for(int i = 0; i < 512; i++){
        args[i] = malloc(512 * sizeof(char));
        commands[i] = malloc(512 * sizeof(char));
    }
    int isExit = 0;
    char output[1024];
    char *buff;
    char *command = malloc(512 * sizeof(char));
    char *arg = malloc(512 * sizeof(char));
    int start = 0;
    int count_com = 0;
    while(!isExit){
        write(STDOUT_FILENO, "> ", 2);
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
                parse_args(&arg, command);

                for (int i = 0; i < strlen(command); i++){
                    if(command[i] == ' ') {
                        command[i] = 0;
                        break;
                    }
                }
                strcpy(args[count_com], arg);
                strcpy(commands[count_com++], command);

                printf("parsed args: %s\n",arg);
                start = i;
            }
        }
        for (int i = 0; i < count_com; i++){
            printf("command %s (args %s) (count = %i)\n",commands[i],args[i], count_com);
        }
        recursive_call(commands, args, count_com);
        start = 0;
        count_com = 0;
    }

}