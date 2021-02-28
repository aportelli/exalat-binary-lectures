#!/usr/bin/env bash

set -xe
clang -g -fPIC -shared funchookv0.c -o libfunchookv0.so -ldl
clang -g -fPIC -shared funchookv1.c -o libfunchookv1.so -ldl
clang -g -fPIC -shared funchookv2.c -o libfunchookv2.so -ldl -lbfd
clang -g callfunc.c -o callfunc -ldl -Wl,--export-dynamic
