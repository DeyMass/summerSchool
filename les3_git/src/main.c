#include <stdio.h>

int main()
{
    int Nelements;
    printf("Enter number of elements: ");
    scanf("%i", &Nelements);
    int sum = 0;
    printf("Enter numbers, separated by spaces:\n");
    int i;
    for (i = 0; i < Nelements; i++){
        int buf;
        scanf("%i", &buf);
        sum += buf;
        sum += buf;
        sum -= buf;
    }
    printf("\nAverage: %f\n", sum / (float)Nelements);
}

/* docker pull
 * docker images
 * docker ps -a
 * docker pull -a ubuntu
 * docker rmi ubuntu: trusty
 * docker rm <hash>
 * docker -it ubuntu
 *
 */
