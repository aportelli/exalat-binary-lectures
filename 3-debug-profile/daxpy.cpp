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
for (unsigned int b = 0; b < nit; ++b)\
{\
    timeStamp    start, end;\
    duration     elapsed;\
    \
    start = hrClock::now();\
    for (unsigned int j = 0; j < nVec; ++j)\
    {\
        __VA_ARGS__;\
    }\
    end     = hrClock::now();\
    elapsed = end - start;\
    if (b == nit - 1)\
    {\
        double gflops = 2.*vecSize*nVec/1.0e9/elapsed.count();\
        double gbytes = 3.*vecSize*nVec*sizeof(double)/1024./1024./1024./elapsed.count();\
        \
        printf("%20s %10.2f %10.2f\n", title, gflops, gbytes);\
    }\
}

// type aliases for timers
typedef chrono::high_resolution_clock hrClock;
typedef chrono::time_point<hrClock>   timeStamp;
typedef chrono::duration<double>      duration;

void daxpy_ptr(const double a, const double *x, double *y, const size_t size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        y[i] = a*x[i] + y[i];
    }
}

void daxpy_ptr_thread(const double a, const double *x, double *y, const size_t size)
{
    #pragma omp parallel for schedule(static)
    for (unsigned int i = 0; i < size; ++i)
    {
        y[i] = a*x[i] + y[i];
    }
}

void daxpy_vec(const double a, const vector<double> &x, vector<double> &y)
{
    for (unsigned int i = 0; i < x.size(); ++i)
    {
        y[i] = a*x[i] + y[i];
    }
}

void daxpy_vec_thread(const double a, const vector<double> &x, vector<double> &y)
{
    #pragma omp parallel for
    for (unsigned int i = 0; i < x.size(); ++i)
    {
        y[i] = a*x[i] + y[i];
    }
}

int main(int argc, char *argv[])
{
    size_t vecSize, nVec;

    if (argc != 3)
    {
        cerr << "usage: " << argv[0] << " <vector size> <number of vectors>" << endl;

        return EXIT_FAILURE;
    }
    vecSize = stoll(argv[1]);
    nVec    = stoll(argv[2]);

#ifdef NARRAY
    vecSize = NARRAY;
    cout << "vector size overriden to " << NARRAY << endl;
#endif

    random_device          rd;
    mt19937                rng(rd());
    normal_distribution<>  gauss;
    vector<double>         x(vecSize), y(vecSize);

#ifdef NARRAY
    double xar[NARRAY], yar[NARRAY];
#endif

    cout << "# vector size " << vecSize << " (" << 2*vecSize*sizeof(double)/1024./1024./1024. << " GB)" << endl;
    cout << "# columns: routine | GFlop/s | GB/s" << endl;
    #pragma omp parallel for
    for (unsigned int i = 0; i < vecSize; ++i)
    {
        x[i] = gauss(rng);
        y[i] = gauss(rng);
        
#ifdef NARRAY
        xar[i] = x[i];
        yar[i] = y[i];
#endif
    }
    
    BENCHMARK("daxpy_heap", 3, daxpy_ptr(2.4564, x.data(), y.data(), vecSize));
#ifdef NARRAY
    BENCHMARK("daxpy_stack", 3, daxpy_ptr(2.4564, xar, yar, NARRAY));
#endif
    BENCHMARK("daxpy_vec", 3, daxpy_vec(2.4564, x, y));
    BENCHMARK("daxpy_mkl", 3, cblas_daxpy(vecSize, 2.4564, x.data(), 1, y.data(), 1));
    BENCHMARK("daxpy_heap_mt", 3, daxpy_ptr_thread(2.4564, x.data(), y.data(), vecSize));
#ifdef NARRAY
    BENCHMARK("daxpy_stack_mt", 3, daxpy_ptr_thread(2.4564, xar, yar, NARRAY));
#endif
    BENCHMARK("daxpy_vec_mt", 3, daxpy_vec_thread(2.4564, x, y));


    return EXIT_SUCCESS;
}