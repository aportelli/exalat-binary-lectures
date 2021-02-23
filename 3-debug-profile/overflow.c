#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    unsigned int l;
    char         *s = argv[1];

    for (l = strlen(s); l >= 0; l--)
    {
        putchar(s[l]);
    }
    putchar('\n');

    return EXIT_SUCCESS;
}