#!/bin/bash

gcc -fopenmp -o mcs-par mcs-par.c

# Array of thread counts to test
THREAD_COUNTS=(2 4 8 16)

# Run the program for each thread count
for THREADS in "${THREAD_COUNTS[@]}"; do
    echo -e "\nRunning with $THREADS threads..."
    export OMP_NUM_THREADS=$THREADS
    ./mcs-par
done
