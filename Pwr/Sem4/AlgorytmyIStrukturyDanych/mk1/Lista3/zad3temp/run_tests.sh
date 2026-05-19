#!/bin/bash

g++ select.cpp -O2 -o select
g++ generator.cpp -O2 -o gen

echo "n,group,comparisons,swaps,rec_calls,time_ms" > results.csv


N_START=100
N_END=50000
STEP=100
M=50

for g in 3 5 7 9 19 21
do
    echo "GROUP_SIZE=$g"

    for ((n=N_START; n<=N_END; n+=STEP))
    do
        total_comp=0
        total_swaps=0
        total_rec=0
        total_time=0

        for ((i=1; i<=M; i++))
        do
            ./gen $n $((n/2)) > input.txt

            out=$(./select $g < input.txt)

            comp=$(echo "$out" | grep "Comparisons" | awk '{print $2}')
            swaps=$(echo "$out" | grep "Swaps" | awk '{print $2}')
            rec=$(echo "$out" | grep "Recursive_calls" | awk '{print $2}')
            time=$(echo "$out" | grep "Time" | awk '{print $2}')

            total_comp=$((total_comp + comp))
            total_swaps=$((total_swaps + swaps))
            total_rec=$((total_rec + rec))
            total_time=$(echo "$total_time + $time" | bc -l)
        done

        avg_comp=$((total_comp / M))
        avg_swaps=$((total_swaps / M))
        avg_rec=$((total_rec / M))
        avg_time=$(echo "$total_time / $M" | bc -l)

        echo "$n,$g,$avg_comp,$avg_swaps,$avg_rec,$avg_time" >> results.csv
    done
done

echo "DONE"