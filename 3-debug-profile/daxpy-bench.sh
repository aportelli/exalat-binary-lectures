#!/usr/bin/env bash

if (( $# != 1 )); then
    echo "usage: `basename $0` <limit>" 1>&2
    exit 1
fi
TOTAL=$1
LIMIT=$1
SIZE=4

while (( $SIZE <= $LIMIT )); do
  clang++ -fopenmp daxpy.cpp -O3 -o daxpy -march=skylake-avx512 -DNARRAY=${SIZE} -lmkl_rt
  NVEC=$(( ${TOTAL}/${SIZE} ))
  OMP_PLACES='cores(12)' OMP_NUM_THREADS=12 OMP_PROC_BIND=close numactl -m 0 ./daxpy ${SIZE} ${NVEC}
  SIZE=$(( 2*${SIZE} ))
done
