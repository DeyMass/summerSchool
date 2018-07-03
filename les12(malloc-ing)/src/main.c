//
// Created by michail on 02.07.18.
//

#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
/*
static void *my_malloc_hook(size_t, const void*);
static void my_init_hook(void);

static void *(*old_malloc_hook)(size_t, const void *);
void (*volatile __malloc_initialize_hook) (void) = my_init_hook;

static void
my_init_hook (void){
    old_malloc_hook = __malloc_hook;
    __malloc_hook = my_malloc_hook;
}

static void *
my_malloc_hook(size_t size, const void * caller){
    void *result;
    __malloc_hook = old_malloc_hook;
    result = malloc(size);
    old_malloc_hook = __malloc_hook;
    printf("malloc(%u) called from %p returned %p", size, caller, result);
    __malloc_hook = my_malloc_hook;
    return result;
}
*/

#define IS_FREE     0
#define IS_NOT_FREE 1

struct malloc_chunk{
    void* ptr;
    int   size;
    int   isFree;
}chunk[65535];

int total_allocated = 0;
int chunk_index = 0;

void get_index(void *ptr){

}

void good_free(void* ptr)
{
    int i;
    for(i = 0; i < chunk_index; i++){
        if (chunk[chunk_index].ptr == ptr) break;
    }
    i--;
    printf("free %i bytes at %i seg\n",chunk[i].size, i);
    chunk[i].isFree = IS_FREE;
    if (i == chunk_index) {
        sbrk(-chunk[i].size);
        total_allocated -= chunk[i].size;
        chunk_index--;
    }
}

void *good_malloc(size_t size)
{
    int alloc_index = chunk_index;
    for (int i = 0; i < chunk_index; i++){
        if (chunk[i].isFree == IS_FREE && chunk[i].size <= size) {
            printf("founded %i b on %i segment\n", size, i);
            alloc_index = i;
            chunk[alloc_index].isFree = IS_NOT_FREE;
            chunk[alloc_index].size = size;
            return chunk[alloc_index].ptr;
        }
    }
    printf("%i bytes UP\n",size);
    chunk[chunk_index].size = size;
    chunk[chunk_index].ptr = sbrk(size);
    chunk[chunk_index].isFree = IS_NOT_FREE;
    return chunk[chunk_index++].ptr;
}

extern void *__libc_malloc(size_t size);
int hook_active = 0;

void* my_not_deprecated_malloc(size_t size, void* caller)
{
    void *result;
    hook_active = 0;
    result = malloc(size);
    printf("logging hook\n");
    hook_active = 1;
    return result;
}

void* malloc(size_t size)
{
    void *caller = __builtin_return_address(0);
    if (hook_active)
        return my_not_deprecated_malloc(size, caller);
    return __libc_malloc(size);
}

int main()
{
    hook_active = 1;
    int *ptr = good_malloc(32);
    int *ptr2 = good_malloc(32);
    int *ptr3 = good_malloc(32);
    good_free(ptr2);
    int *ptr4 = good_malloc(32);

    for (int i = 0; i < 40; i++){
        ptr[i] = 0;
        ptr3[i] = 0;
        ptr4[i] = 0;
        printf("%i OK\n", i);
    }
    good_free(ptr);
    good_free(ptr3);
    good_free(ptr4);
    printf("total allocated %i\n",total_allocated);
}