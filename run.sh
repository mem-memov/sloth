#!/bin/bash

set -e -u

ATTEMPT=1

set
while [[ "${ATTEMPT}" -lt 2 ]]; do

    gcc main.c -o main && ./main

    ATTEMPT=$(( ${ATTEMPT} + 1 ))

done
