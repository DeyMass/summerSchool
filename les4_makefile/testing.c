#include <math.h>
#include <stdio.h>

int myPow(int a){
    return a*a;
}

int main(){
    int i;
    for (i = 0; i < 12345; i++){
        if (myPow(i) != pow(i, 2)){
            printf("ERROR in pow\n");
            return -1;
        }
    }
    printf ("SUCCESS\n");
    return 0;
}
