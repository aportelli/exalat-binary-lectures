#!/usr/bin/env bash

nasm -f elf64 jump.asm -o jump.o                   # assemble
objcopy -O binary jump.o jump.bin                  # extract binary section
hexdump -v -e '"\\""x" 1/1 "%02x" ""' jump.bin     # convert to C string
rm -f jump.bin jump.o                              # cleanup
