#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int  x0 = 1, x1 = 1, xn, n;
    char buffer[16];

    strcpy(buffer, argv[1]);
    printf("Starting Fibonacci sequence with label %s\n", buffer);
    printf("%d %d ", x0, x1);
    for (n = 0; n < 10; ++n)
    {
        xn = x1 + x0;
        printf("%d ", xn);
        x0 = x1;
        x1 = xn;
    }
    printf("\n");

    return EXIT_SUCCESS;
}