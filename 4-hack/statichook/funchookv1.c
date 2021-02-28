#include <stdio.h>
#include <dlfcn.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

extern int errno;

void replacement_func(const int id)
{
    printf("calling variant with id %d\n", id);
}

__attribute__((constructor)) static void ctor(void) 
{
	// get pointers to the original and new functions and calculate the jump offset
    // only works if the main program exports its symbols! (cf. v2 for an alternative)
    void *main_program_handle = dlopen(NULL, RTLD_NOW);
    void *target_func         = dlsym(main_program_handle, "func");
    if (!target_func)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    void *new_func = &replacement_func;

    // make the memory containing the original funcion writable
    // the address passed to mprotect must be aligned on page boundary
    size_t    page_size  = sysconf(_SC_PAGESIZE);
    uintptr_t start      = (uintptr_t)target_func;
    uintptr_t end        = start + 1;
    uintptr_t page_start = start & -page_size;
    int status = mprotect((void *)page_start, end - page_start, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (status)
    {
        fprintf(stderr, "Error unprotecting page\n");
        fprintf(stderr, "Value of errno    : %d\n", errno);
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // skeleton for shellcode
    unsigned char shellcode[] = "\x49\xba\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\x41\xff\xe2";
    size_t code_size = sizeof(shellcode) - 1;

    // copy address into shellcode and inject
    memcpy(shellcode + 2, &new_func, sizeof(uint64_t));
    memcpy(target_func, shellcode, code_size);

    // summary
    printf("====================\n");
    printf("INJECTION SUCCESSFUL\n");
    printf("Injected code   : ");
    for (unsigned int i = 0; i < code_size; ++i)
    {
        printf("\\x%x", shellcode[i]);
    }
    printf("\n");
    printf("Target function : %p\n", target_func);
    printf("New function    : %p\n", new_func);
    printf("====================\n");
}
