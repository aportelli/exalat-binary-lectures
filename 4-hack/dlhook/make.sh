#!/usr/bin/env bash

set -xe
clang -g -fPIC -shared func1.c -o libfunc1.so
clang -g -fPIC -shared func2.c -o libfunc2.so
clang -g -fPIC -shared funchook.c -o libfunchook.so -ldl
clang -g callfunc.c -o callfunc -lfunc1 -L.
