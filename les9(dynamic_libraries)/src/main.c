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
    char pluginsList[1024];//строка с именами библиотек, разделённых запятыми
    printf("Enter libraries to open:\n");
    scanf("%s", pluginsList);
    char  buff[64];//буффер с именем библиотеки
    int i = 0;//Итератор по строке с именами библиотек
    int nameLength = 0;//длина имени файла
    int dlCount = 0;//Число подгруженных библиотек
    void* dlDesc[16];//массив дескрипторов динамических библиотек
    char* funcName;//Реальное имя функции из библиотеки
    char* funcDescription[20];//Описание функций из библиотеки
    float ((*function[20]))(float,float);//массив указателей на функции из библиотек
    while(1){//пока не достигнут конец файла
        if (pluginsList[i] == ',' || pluginsList[i] == ' ' || pluginsList[i] == 0){ //признак конца имени файла
            buff[nameLength] = 0;
            nameLength = 0;
            dlDesc[dlCount] = dlopen(buff, RTLD_NOW);
            if(dlDesc[dlCount] != NULL){//Если библиотека найдена
                funcDescription[dlCount] = dlsym(dlDesc[dlCount], "description");//считать секцию описания из библиотеки
                funcName = dlsym(dlDesc[dlCount], "funcName");//считать секцию имени функций из библиотеки
                function[dlCount] = dlsym(dlDesc[dlCount], funcName);//считать по имени адрес функции из библиотеки
                dlCount++;//Счётчик подключенных библиотек
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
            printf("%i) %s\n", iterator, funcDescription[iterator-1]); //вывод списка описаний
        }
        printf("Select operation:\n");
        scanf("%i",&operation);
        if(operation > dlCount || operation <= 0){//Если такой операции не существует, то завершить программу
            printf("No such operation. Stopping program...");
            return 0;
        }
        printf("Enter first operand:");
        scanf("%f", &operandA);
        printf("Enter second operand:");
        scanf("%f", &operandB);
        result = function[operation - 1](operandA, operandB);
        printf("Result = %f\nPRESS ANY KEY...",result);
        dump_line(stdin);
        getchar();
        system("clear");
    }
    return 0;
}
