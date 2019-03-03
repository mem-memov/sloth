#!/bin/bash

gcc main.c -o main

ATTEMPT=1

while [[ "${ATTEMPT}" -lt 1000 ]]; do

    echo ${ATTEMPT}

    ATTEMPT=$(( ${ATTEMPT} + 1 ))

    ./main &

    #sleep 1

done

wait 
echo "All done"
