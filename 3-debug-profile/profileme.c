#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define vsize 20000
typedef double vec[vsize];

void vec_copy(vec v, const vec w)
{
    for (unsigned int i = 0; i < vsize; ++i)
    {
        v[i] = w[i];
    }
}

void vec_acos(vec v)
{
    for (unsigned int i = 0; i < vsize; ++i)
    {
        v[i] = acos(v[i]);
    }
}

void vec_sin(vec v)
{
    for (unsigned int i = 0; i < vsize; ++i)
    {
        v[i] = sin(v[i]);
    }
}

void vec_acc(vec v, const vec w)
{
    for (unsigned int i = 0; i < vsize; ++i)
    {
        v[i] += w[i];
    }
}

void vec_rand(vec v)
{
    time_t t;

    srand((unsigned)time(&t));
    for (unsigned int i = 0; i < vsize; ++i)
    {
        v[i] = (double)(rand() % 50)/50.;
    }
}

int main(void)
{
    vec    v, w, tmp;
    
    vec_rand(v);
    vec_copy(w, v);
    for (unsigned int i = 0; i < 10000; ++i)
    {
        vec_acos(w);
        vec_sin(v);
        vec_acc(v, w);
    }

    return EXIT_SUCCESS;
}
