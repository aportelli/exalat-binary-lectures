#!/usr/bin/env gnuplot -p

set term qt font 'arial,14'

set title 'daxpy Xeon 4116 benchmark'
set xlabel 'vector length'
set ylabel 'memory bandwith (GB/s)'

set label 'DRAM peak' at 5e6, 140
set label 'L3 cache size (16.5 MB)' at 1e4, 675

# category10 colors
set linetype  1 lc rgb "#1f77b4" lw 1.5
set linetype  2 lc rgb "#ff7f0e" lw 1.5
set linetype  3 lc rgb "#2ca02c" lw 1.5
set linetype  4 lc rgb "#d62728" lw 1.5
set linetype  5 lc rgb "#9467bd" lw 1.5
set linetype  6 lc rgb "#8c564b" lw 1.5
set linetype  7 lc rgb "#e377c2" lw 1.5
set linetype  8 lc rgb "#7f7f7f" lw 1.5
set linetype  9 lc rgb "#bcbd22" lw 1.5

set logscale x

plot '< ./parse-daxpy-bench.sh daxpy-bench.log daxpy_heap' u 1:3 w lines t 'heap',\
     '< ./parse-daxpy-bench.sh daxpy-bench.log daxpy_stack' u 1:3 w lines t 'stack',\
     '< ./parse-daxpy-bench.sh daxpy-bench.log daxpy_mkl' u 1:3 w lines t 'MKL',\
     '< ./parse-daxpy-bench.sh daxpy-bench.log daxpy_heap_mt' u 1:3 w lines t 'heap MT',\
     '< ./parse-daxpy-bench.sh daxpy-bench.log daxpy_stack_mt' u 1:3 w lines t 'stack MT',\
     107.3 w lines lc rgb 'black' dt 2 notitle,\
     '< printf "1081344 0\n1081344 700"' u 1:2 lc rgb 'black' dt 2 w lines notitle