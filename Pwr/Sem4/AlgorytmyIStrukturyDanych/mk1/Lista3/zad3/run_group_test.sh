#!/usr/bin/env bash

GEN="./generator"
SELECT="./select"

OUT="group_results.csv"

echo "group,n,comparisons,swaps,time" > $OUT

M=50

for g in 3 5 7 9 19 21
do
    echo "Testing group size = $g"

    for ((n=100; n<=50000; n+=1000))
    do

        k=$((n/2))

        for ((i=1; i<=M; i++))
        do

            data=$($GEN $n $k)

            result=$(
                echo "$data" | $SELECT $g
            )

            comps=$(
                echo "$result" |
                grep "Comparisons" |
                awk '{print $2}'
            )

            swaps=$(
                echo "$result" |
                grep "Swaps" |
                awk '{print $2}'
            )

            time=$(
                echo "$result" |
                grep "Time" |
                awk '{print $2}'
            )

            echo "$g,$n,$comps,$swaps,$time" >> $OUT

        done
    done
done

echo "DONE"