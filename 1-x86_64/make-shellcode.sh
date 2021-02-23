#!/usr/bin/env bash

nasm -f elf64 hello-pi.asm -o hello-pi.o           # assemble
ld hello-pi.o -o hello-pi                          # link
objcopy -O binary hello-pi hello-pi.bin            # extract binary section
hexdump -v -e '"\\""x" 1/1 "%02x" ""' hello-pi.bin # convert to C string
rm -f hello-pi.bin hello-pi.o                      # cleanup
