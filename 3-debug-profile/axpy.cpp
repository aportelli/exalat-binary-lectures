#include <iostream>
#include <chrono>
#include <random>
#include <vector>

using namespace std;

#define BENCHMARK(title, ...)\
cout << "-- benchmarking " << title << endl;\
for (unsigned int b = 0; b < 10; ++b)\
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
    cout << "#" << b << " duration= " << elapsed.count() << " s -- ";\
    cout << "Gflop/s= " << 2.*vecSize*nVec/1.0e9/elapsed.count();\
    cout << " GB/s= " << 2.*vecSize*nVec*sizeof(double)/1024./1024./1024./elapsed.count() << endl;\
}


// type aliases for timers
typedef chrono::high_resolution_clock hrClock;
typedef chrono::time_point<hrClock>   timeStamp;
typedef chrono::duration<double>      duration;

void axpy_ptr(const double a, const double *x, double *y, const size_t size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        y[i] = a*x[i] + y[i];
    }
}

void axpy_ptr_thread(const double a, const double *x, double *y, const size_t size)
{
    #pragma parallel omp for
    for (unsigned int i = 0; i < size; ++i)
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

    random_device          rd;
    mt19937                rng(rd());
    normal_distribution<>  gauss;
    vector<double>         x(vecSize), y(vecSize);

    cout << "-- generating random vectors of size " << vecSize << endl;
    cout << "total size " << 2*vecSize*sizeof(double)/1024./1024./1024. << " GB" << endl;
    for (unsigned int i = 0; i < vecSize; ++i)
    {
        x[i] = gauss(rng);
        y[i] = gauss(rng);
    }

    // cout << "-- benchmarking " << endl;
    // for (unsigned int b = 0; b < 10; ++b)
    // {
    //     timeStamp    start, end;
    //     duration     elapsed;

    //     start = hrClock::now();
    //     for (unsigned int j = 0; j < nVec; ++j)
    //     {
    //         axpy_ptr(2.4564, x.data(), y.data(), vecSize);
    //     }
    //     end     = hrClock::now();
    //     elapsed = end - start;
    //     cout << "#" << b << " duration= " << elapsed.count() << " s -- ";
    //     cout << "Gflop/s= " << 2.*vecSize*nVec/1.0e9/elapsed.count();
    //     cout << " GB/s= " << 2.*vecSize*nVec*sizeof(double)/1024./1024./1024./elapsed.count() << endl;
    // }
    
    BENCHMARK("axpy_ptr", axpy_ptr(2.4564, x.data(), y.data(), vecSize));
    BENCHMARK("axpy_ptr_thread", axpy_ptr_thread(2.4564, x.data(), y.data(), vecSize));

    return EXIT_SUCCESS;
}