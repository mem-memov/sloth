#!/bin/bash

ATTEMPT=1

set
while [[ "${ATTEMPT}" -lt 100 ]]; do

    ATTEMPT=$(( ${ATTEMPT} + 1 ))

    gcc main.c -o main && ./main &

done

wait 
echo "All done"
