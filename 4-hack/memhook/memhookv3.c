#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <execinfo.h>

static size_t peak = 0, current = 0, cumul = 0;

#define MAX_STACK 256
#define PRINT_BACKTRACE \
{\
    void* addrlist[MAX_STACK+1];\
    int addrlen;\
    \
    printf("BACKTRACE\n");\
    addrlen = backtrace(addrlist, (int)(sizeof(addrlist)/sizeof(void*)));\
    if (addrlen == 0)\
    {\
        printf("error: empty back trace\n");\
    }\
    else\
    {\
        backtrace_symbols_fd(addrlist, addrlen, fileno(stdout));\
    }\
}

void *malloc(size_t size)
{
    static void *(*real_malloc)(size_t) = NULL;
    static int  in_hook = 0;

    if (!real_malloc)
    {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        printf("Target function malloc loaded at %p\n", real_malloc);
    }
    
    void *p = real_malloc(size);

    if (!in_hook)
    {
        size_t real_size = malloc_usable_size(p); 

        in_hook  = 1;
        current += real_size;
        cumul   += real_size;
        peak     = (current > peak) ? current : peak;
        printf("==== MEM DEBUG ====\nAllocated %zdB at %p (current %zdB, peak %zdB, cumulative %zdB)\n",
               real_size, p, current, peak, cumul);
        PRINT_BACKTRACE
        printf("===================\n");
        in_hook  = 0;
    }

    return p;
}


void free(void *p)
{
    static void *(*real_free)(void *) = NULL;
    static int in_hook = 0; 

    if (!real_free)
    {
        real_free = dlsym(RTLD_NEXT, "free");
        printf("Target function free loaded at %p\n", real_free);
    }
    if (p)
    {
        real_free(p);
        if (!in_hook)
        {
            size_t size = malloc_usable_size(p);

            in_hook = 1;
            current -= size;
            printf("==== MEM DEBUG ====\nFreed %zdB at %p (current %zdB, peak %zdB, cumulative %zdB)\n",
                   size, p, current, peak, cumul);
            PRINT_BACKTRACE;
            printf("===================\n");
            in_hook = 0;
        }
    }
}
