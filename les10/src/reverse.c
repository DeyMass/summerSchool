#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    int fdRead = open(argv[1], O_RDONLY);
    int fdWrite = open(argv[2], O_CREAT, S_IRWXO | S_IRWXG | S_IRWXU);
    if (fdRead < 0 || fdWrite < 0){
        printf("ERROR");
        return -1;
    }
    close(fdWrite);
    fdWrite = open(argv[2], O_WRONLY, 666);
    printf("reading from: %s\nwrite to: %s", argv[1], argv[2]);
    int i;
    char buffer[15];
    int readed = 1;
    int size;
    while(readed > 0){
        readed = read(fdRead, buffer, 3);
        if (readed == 0) break;
        size += readed;
        printf("\nReaded %i bytes", readed);
    }
    for (int i = 1; i <= size; i++){
        lseek(fdRead, -i, SEEK_END);
        if (read(fdRead, buffer, 1) == 0) break;
        write(fdWrite, buffer, 1);
    }
    printf("\n");
}
