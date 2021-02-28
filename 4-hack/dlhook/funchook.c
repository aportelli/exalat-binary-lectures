// RTLD_NEXT is a GNU extension
#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void func(const int id)
{
    // static pointer to the target
    static void (*target_func)(const int) = NULL;

    if (!target_func)
    {
        // if the target pointer is NULL, load it using dlsym from libdl
        target_func = dlsym(RTLD_NEXT, "func");
        printf("Target loaded at %p\n", target_func);
    }

    // wrapper
    printf("Do some things before func call...\n");
    target_func(id);
    printf("Do some things after func call...\n");
}
