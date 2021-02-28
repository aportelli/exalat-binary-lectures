#!/usr/bin/env bash

set -xe
clang -g -fPIC -shared memhookv1.c -o libmemhookv1.so -ldl
clang -g -fPIC -shared memhookv2.c -o libmemhookv2.so -ldl
clang -g -fPIC -shared memhookv3.c -o libmemhookv3.so -ldl
