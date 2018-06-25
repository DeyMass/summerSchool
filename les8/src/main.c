#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathFunctions.h"
#include "stdbool.h"

void dump_line(FILE * fp) {
    int ch;
    while ((ch = fgetc(fp)) != EOF && ch != '\n');
}



int menu(int *isInterrupted){
    int   operation;
    float operandA, operandB, result;
    printf("Select operation:\n"
           "1) Add\n"
           "2) Sub\n"
           "3) Divide\n"
           "4) Multiply\n"
           "5) Log by base\n"
           "6) Exit\n");
    scanf("%i", &operation);
    if (operation == 6){
        *isInterrupted = true;
        return -1;
    }
    printf("First operand:\n");
    scanf("%f", &operandA);
    printf("Second operand:\n");
    scanf("%f", &operandB);
    result = 0;
    switch(operation){
        case 1:
            result = sum(operandA, operandB);
            break;
        case 2:
            result = sub(operandA, operandB);
            break;
        case 3:
            result = divide(operandA, operandB);
            break;
        case 4:
            result = mul(operandA, operandB);
            break;
        case 5:
            result = op_log(operandA, operandB);
            break;
        default:
            printf("Unknown operation");
    }
    return result;
}

int main()
{
    int   isInterrupted = 0;
    float result = 0;

    while(!isInterrupted){
        system("clear");
        result = menu(&isInterrupted);
        printf("Result = %f\nPRESS ANY KEY... ", result);
        dump_line(stdin);
        getchar();
    }
    return 0;
}
