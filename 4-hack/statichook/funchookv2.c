#include <stdio.h>
#include <dlfcn.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <bfd.h>

extern int errno;

void replacement_func(const int id)
{
    printf("calling variant with id %d\n", id);
}

// find symbol using BFD
int64_t * find_symbol(char *filename, char *symname)
{
    bfd         *ibfd;
    asymbol     **symtab;
    long        nsize, nsyms, i;
    symbol_info syminfo;
    char        **matching;

    bfd_init();

    ibfd = bfd_openr(filename, NULL);

    bfd_check_format_matches(ibfd, bfd_object, &matching);

    nsize  = bfd_get_symtab_upper_bound (ibfd);
    symtab = malloc(nsize);
    nsyms  = bfd_canonicalize_symtab(ibfd, symtab);

    for (i = 0; i < nsyms; i++) 
    {
        if (strcmp(symtab[i]->name, symname) == 0) 
        {
            bfd_symbol_info(symtab[i], &syminfo);
            return (int64_t *) syminfo.value;
        }
    }

    bfd_close(ibfd);
    fprintf(stderr, "error: cannot find symbol\n");

    return NULL;
}

__attribute__((constructor)) static void ctor(void) 
{
	// get pointers to the original and new functions
    // works even if the main program does not export symbols
    int64_t *target_func = find_symbol("./callfunc", "func");
    int64_t *new_func    = (int64_t*)&replacement_func;

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

