#!/bin/bash

GEN="./generator"
RAND="./randomized_select"
SELECT="./select"

M=50

OUT_RAND="randomized_results.csv"
OUT_SELECT="select_results.csv"

echo "n,k,comparisons,swaps" > $OUT_RAND
echo "n,k,comparisons" > $OUT_SELECT

for ((n=100; n<=50000; n+=100)); do
    echo "Testing n=$n"

    for k in 1 $((n/4)) $((n/2)) $((3*n/4)) $n; do

        for ((i=1; i<=M; i++)); do

            data=$($GEN $n $k)

            result=$(
                echo "$data" | $RAND
            )

            comps=$(echo "$result" | grep "Comparisons" | awk '{print $2}')
            swaps=$(echo "$result" | grep "Swaps" | awk '{print $2}')

            echo "$n,$k,$comps,$swaps" >> $OUT_RAND

            result2=$(
                echo "$data" | $SELECT
            )

            comps2=$(echo "$result2" | grep "Comparisons" | awk '{print $2}')

            echo "$n,$k,$comps2" >> $OUT_SELECT

        done
    done
done

echo "Done."