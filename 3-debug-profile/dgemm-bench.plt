#!/usr/bin/env gnuplot -p

set term qt font 'arial,14'

set title 'dgemm Xeon 4116 benchmark'
set xlabel 'matrix size'
set ylabel 'FP performance (GFlop/s)'
set key left center


set label 'FP peak' at 1000, 550

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

plot '< ./parse-dgemm-bench.sh dgemm-bench.log dgemm_heap' u 1:2 w lines t 'heap',\
     '< ./parse-dgemm-bench.sh dgemm-bench.log dgemm_stack' u 1:2 w lines t 'stack',\
     '< ./parse-dgemm-bench.sh dgemm-bench.log dgemm_mkl' u 1:2 w lines t 'MKL',\
     '< ./parse-dgemm-bench.sh dgemm-bench.log dgemm_heap_mt' u 1:2 w lines t 'heap MT',\
     '< ./parse-dgemm-bench.sh dgemm-bench.log dgemm_stack_mt' u 1:2 w lines t 'stack MT',\
     567 w lines lc rgb 'black' dt 2 notitle