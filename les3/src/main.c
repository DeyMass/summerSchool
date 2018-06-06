#include <stdio.h>

int main()
{
    int Nelements;
    scanf("%i", &Nelements);
    int sum = 0;
    for (int i = 0; i < Nelements; i++){
        int buf;
        scanf("%i", &buf);
        if (buf > 0) sum += buf;
        else sum += buf;
    }
    printf("\nAverage: %f\n", sum / (float)Nelements);
}
