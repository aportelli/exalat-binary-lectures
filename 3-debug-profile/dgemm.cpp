#include <iostream>
#include <chrono>
#include <random>
#include <array>
#include <vector>
#include <omp.h>
#include <cassert>
#include "mkl.h"
#include "mkl_cblas.h"

using namespace std;

#define BENCHMARK(title, nit, ...)\
for (unsigned int bench = 0; bench < nit; ++bench)\
{\
    timeStamp    start, end;\
    duration     elapsed;\
    \
    start = hrClock::now();\
    for (unsigned int j = 0; j < nMat; ++j)\
    {\
        __VA_ARGS__;\
    }\
    end     = hrClock::now();\
    elapsed = end - start;\
    if (bench == nit - 1)\
    {\
        double gflops = n*n*(2*n-1.)*nMat/1.0e9/elapsed.count();\
        double gbytes = 3.*n*n*nMat*sizeof(double)/1024./1024./1024./elapsed.count();\
        \
        printf("%20s %10.2f %10.2f\n", title, gflops, gbytes);\
    }\
}

// type aliases for timers
typedef chrono::high_resolution_clock hrClock;
typedef chrono::time_point<hrClock>   timeStamp;
typedef chrono::duration<double>      duration;

void dgemm_ptr(double *c, const double *a, const double *b, const size_t n)
{
    for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j)
    {
        c[i*n + j] = a[i*n]*b[j];
    }
    for (unsigned int i = 0; i < n; ++i)
    for (unsigned int k = 1; k < n; ++k)
    for (unsigned int j = 0; j < n; ++j)
    {
        c[i*n + j] += a[i*n + k]*b[k*n + j];
    }
}

void dgemm_ptr_thread(double *c, const double *a, const double *b, const size_t n)
{
    #pragma omp parallel for
    for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j)
    {
        c[i*n + j] = a[i*n]*b[j];
    }

    #pragma omp parallel for
    for (unsigned int i = 0; i < n; ++i)
    for (unsigned int k = 1; k < n; ++k)
    for (unsigned int j = 0; j < n; ++j)
    {
        c[i*n + j] += a[i*n + k]*b[k*n + j];
    }
}

void dgemm_mkl(double *c, const double *a, const double *b, const size_t n)
{
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n,
                n, 1., a, n, b, n, 0., c, n);
}

int main(int argc, char *argv[])
{
    size_t n, nMat;

    if (argc != 3)
    {
        cerr << "usage: " << argv[0] << " <N> <number of matrices>" << endl;

        return EXIT_FAILURE;
    }
    n    = stoll(argv[1]);
    nMat = stoll(argv[2]);

#ifdef NARRAY
    n = NARRAY;
    cout << "n overriden to " << NARRAY << endl;
#endif

    random_device          rd;
    mt19937                rng(rd());
    normal_distribution<>  gauss;
    vector<double>         a(n*n), b(n*n), c(n*n);

#ifdef NARRAY
    double aar[NARRAY*NARRAY], bar[NARRAY*NARRAY], car[NARRAY*NARRAY];
#endif

    cout << "# square matrix nxn with n = " << n << " (" << 2*n*n*sizeof(double)/1024./1024./1024. << " GB)" << endl;
    cout << "# columns: routine | GFlop/s | GB/s" << endl;
    #pragma omp parallel for
    for (unsigned int i = 0; i < n*n; ++i)
    {
        a[i] = gauss(rng);
        b[i] = gauss(rng);
#ifdef NARRAY
        aar[i] = a[i];
        bar[i] = b[i];
#endif
    }
    BENCHMARK("dgemm_heap", 3, dgemm_ptr(c.data(), a.data(), b.data(), n));
    BENCHMARK("dgemm_heap_mt", 3, dgemm_ptr_thread(c.data(), a.data(), b.data(), n));
#ifdef NARRAY
    BENCHMARK("dgemm_stack", 3, dgemm_ptr(car, aar, bar, n));
    BENCHMARK("dgemm_stack_mt", 3, dgemm_ptr_thread(car, aar, bar, n));
#endif
    BENCHMARK("dgemm_mkl", 3, dgemm_mkl(c.data(), a.data(), b.data(), n));
    
    return EXIT_SUCCESS;
}