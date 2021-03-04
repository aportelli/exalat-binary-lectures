#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void print_hash(int argc, char** argv, char** env)
{
    // address of hash in crackme.3
    uint64_t      address        = 0x401150;
    // cast to a function pointer
    unsigned int (*hash)(char *) = (unsigned int (*)(char *))(address);

    printf("%s %x\n", argv[1], hash(argv[1]));
    exit(EXIT_SUCCESS);
}

__attribute__((section(".init_array"))) void *init = &print_hash;
