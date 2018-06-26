#include "stdlib.h"
#include "mathFunc.h"
#include "stdio.h"
#include "dlfcn.h"

#define LIB_ERROR   0
#define LIB_SUCCESS 1

//функция для очистки буфера ввода
void dump_line(FILE * fp) {
    int ch;
    while ((ch = fgetc(fp)) != EOF && ch != '\n');
}

int main()
{
    system("clear");
    char pluginsList[1024];
    printf("Enter libraries to open:\n");
    scanf("%s", pluginsList);
    char  buff[64];
    void* dlDesc[16];
    int i = 0;
    int nameLength = 0;
    int dlCount = 0;
    char* funcName;
    char* funcDescription[20];
    char functionNames[1024];
    float ((*function[20]))(float,float);
    while(1){
        if (pluginsList[i] == ',' || pluginsList[i] == ' ' || pluginsList[i] == 0){ //признак конца имени файла
            buff[nameLength] = 0;
            nameLength = 0;
            dlDesc[dlCount] = dlopen(buff, RTLD_NOW);
            if(dlDesc[dlCount] != NULL){//Если библиотека найдена
                funcDescription[dlCount] = dlsym(dlDesc[dlCount], "description");
                funcName = dlsym(dlDesc[dlCount], "funcName");
                function[dlCount] = dlsym(dlDesc[dlCount], funcName);
                dlCount++;
            } else {
                printf("Error while opening \"%s\" because of: \n%s\n", buff, dlerror());
            }
            if (pluginsList[i] == 0) break;
            i++;
        }
        buff[nameLength++] = pluginsList[i++];
    }
    printf("Total opened libraries: %i\n", dlCount);
    int iterator;
    int isInterrupted = 0;
    float result = 0;
    while(!isInterrupted){
        float operandA = 0, operandB = 0;
        int operation;
        for (iterator = 1; iterator <= dlCount; iterator++){
            printf("%i) %s\n", iterator, funcDescription[iterator-1]);
        }
        printf("Select operation:\n");
        scanf("%i",&operation);
        if(operation > dlCount || operation <= 0){
            return 0;
        }
        printf("Enter first operand:");
        scanf("%f", &operandA);
        printf("Enter second operand:");
        scanf("%f", &operandB);
        result = function[operation - 1](operandA, operandB);
        printf("Result = %f\nPRESS ANY KEY...",result);
        getchar();
        getchar();
        system("clear");
    }
    return 0;
}
