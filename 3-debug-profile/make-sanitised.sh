#!/usr/bin/env bash

set -xe
clang -fsanitize=address -fno-omit-frame-pointer -g boverflow.c -o boverflow
clang -fsanitize=address -fno-omit-frame-pointer -g leak.c -o leak
