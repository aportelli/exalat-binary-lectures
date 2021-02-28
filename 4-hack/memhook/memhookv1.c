#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

static size_t peak = 0, current = 0, cumul = 0;

void *malloc(size_t size)
{
    static void *(*real_malloc)(size_t) = NULL;

    if (!real_malloc)
    {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        printf("Target function malloc loaded at %p\n", real_malloc);
    }

    void   *p        = real_malloc(size);
    size_t real_size = malloc_usable_size(p); 

    current += real_size;
    cumul   += real_size;
    peak     = (current > peak) ? current : peak;
    printf("==== MEM DEBUG ==== Allocated %zdB at %p (current %zdB, peak %zdB, cumulative %zdB)\n",
           real_size, p, current, peak, cumul);

    return p;
}

void free(void *p)
{
    static void *(*real_free)(void *) = NULL;

    if (!real_free)
    {
        real_free = dlsym(RTLD_NEXT, "free");
        printf("Target function free loaded at %p\n", real_free);
    }

    real_free(p);

    size_t size = malloc_usable_size(p);

    current -= size;

    printf("==== MEM DEBUG ==== Freed %zdB at %p (current %zdB, peak %zdB, cumulative %zdB)\n",
           size, p, current, peak, cumul);
}
