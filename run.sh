#!/bin/bash

gcc main.c -o main

ATTEMPT=1

while [[ "${ATTEMPT}" -lt 100 ]]; do

    ATTEMPT=$(( ${ATTEMPT} + 1 ))

    ./main &

done

wait 
echo "All done"
