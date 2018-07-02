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

extern void *__libc_malloc(size_t size);
int hook_active = 0;

void* my_not_deprecated_malloc(size_t size, void* caller){
    void *result;
    hook_active = 0;
    result = malloc(size);
    printf("logging hook\n");
    hook_active = 1;
    return result;
}

void* malloc(size_t size){
    void *caller = __builtin_return_address(0);
    if (hook_active)
        return my_not_deprecated_malloc(size, caller);
    return __libc_malloc(size);
}

int main(){
    hook_active = 1;
    int *ptr = malloc(32);
    for (int i = 0; i < 32/4; i++){
        printf("%i\n",ptr[i]);
    }
}