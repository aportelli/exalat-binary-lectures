#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double *make_vector(const size_t size)
{
    return (double *)malloc(size*sizeof(double));
}

void destroy_vector(double * vec)
{
    free(vec);
}

void copy_vector(double *dest, const double *src, const size_t size)
{
    memcpy(dest, src, size*sizeof(double));
}

void swap_vectors(double *a, double *b, size_t size)
{
    double *tmp = make_vector(size);

    copy_vector(tmp, a, size);
    copy_vector(a, b, size);
    copy_vector(b, tmp, size);
}

int main(void)
{
    size_t n = 10;
    double *a, *b;

    a = make_vector(n);
    b = make_vector(n);
    swap_vectors(a, b, n);
    destroy_vector(a);
    destroy_vector(b);

    return EXIT_SUCCESS;
}