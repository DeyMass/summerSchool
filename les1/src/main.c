#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern float sum(float a, float b);
extern float sub(float a, float b);
extern float divide(float a, float b);
extern float mul(float a, float b);
extern float op_log(float base, float val);

int main()
{
    float operandA, operandB, result;
    char operation;
    printf("Select operation:\n"
           "1) Add"
           "2) Sub"
           "3) Multiply\n"
           "4) Divide\n");
    scanf("%f%c%f", &operandA, &operation, &operandB);
    switch(operation){
        case '+':
            result = sum(operandA, operandB);
            break;
        case '-':
            result = sub(operandA, operandB);
            break;
        case '/':
            result = divide(operandA, operandB);
            break;
        case '*':
            result = mul(operandA, operandB);
            break;
        case 'l':
            result = op_log(operandA, operandB);
		    break;
        case 'p':
            result = pow(4, 7);
            break;
        default:
            printf("Unknown operation");
    }
    printf("\nresult = %f\n", result);
    return 0;
}
