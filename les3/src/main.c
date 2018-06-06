#include <stdio.h>

int main()
{
    int Nelements;
    printf("Enter number of elements (strongly positive): ");
    scanf("%i", &Nelements);
    int sum = 0;
    printf("Enter numbers, separated by spaces:\n");
    for (int i = 0; i < Nelements; i++){
        int buf;
        scanf("%i", &buf);
        if (buf > 0)
            sum += buf;
    }
    printf("\nAverage: %f\n", sum / (float)Nelements);
}
