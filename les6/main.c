#include "stdio.h"

//global

//prototypes

int main(){
    //local
    register int cr; //попросить разместить в регистре
    static int a; //static
    const int b; //const
    volatile int c; //может видоизмениться вне программы
    const int * const p; //константный указатель на константу
    //restrict int *d; //уверить компилятор что это указатель на этот участок памяти ни с чем не пересекается

    int test = 111;
    {
        //block
        test++;
        printf("%i\n", test);
        int test = 555;
        test++;
        printf("%i\n", test);
    }
    printf("%i\n", test);

    sizeof(long);
    printf("HelloWorld");
    return 0;
}
