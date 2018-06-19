//
// Created by michail on 19.06.18.
//

#include "stdio.h"

#define ALIGNMENT_ATTRIB     __attribute__((packed))
#define ALIGNED_STRUCT       aligned_structure
#define UNALIGNED_STRUCT     unaligned_structure
#define ALIGNED_S_POINTER    struct ALIGNED_STRUCT*
#define UNALIGNED_S_POINTER  struct UNALIGNED_STRUCT*

struct ALIGNED_STRUCT{
    char symbol;
    int number;
};

struct ALIGNMENT_ATTRIB UNALIGNED_STRUCT{
    char symbol;
    int number;
};

int main()
{
    char str[10] = {'A', 0, 0, 0, 0, 'B', 0, 0, 0, 0};
    struct ALIGNED_STRUCT *pointer = (ALIGNED_S_POINTER) str;
    struct UNALIGNED_STRUCT *sec_pointer = (UNALIGNED_S_POINTER) str;
    printf("Aligned struct: %c %i (sizeof = %lu)", pointer->symbol, pointer->number, sizeof(struct ALIGNED_STRUCT));
    printf("\n");
    printf("Unaligned struct: %c %i (sizeof = %lu)", sec_pointer->symbol, sec_pointer->number, sizeof(struct UNALIGNED_STRUCT));
    printf("\n");

    
}