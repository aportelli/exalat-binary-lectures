#!/usr/bin/env bash

if (( $# != 2 )); then
    echo "usage: `basename $0` <log file> <routine>" 1>&2
    exit 1
fi
LOG=$1
ROUTINE=$2

awk -v rout=${ROUTINE} '/^# vector size/{printf("%12d ", $4)} {if ($1 == rout) {printf("%8.2f %8.2f\n", $2, $3)}}' $LOG