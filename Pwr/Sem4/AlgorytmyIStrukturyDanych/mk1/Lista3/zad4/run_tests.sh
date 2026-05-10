#!/bin/bash

CASE1="./case1"
CASE2="./case2"

OUT1="case1_results.csv"
OUT2="case2_results.csv"

echo "n,smart,binary" > $OUT1
echo "n,smart,binary" > $OUT2

for ((n=1000; n<=300000; n+=1000))
do
    echo "n = $n"

    res1=$($CASE1 <<< "$n")

    smart1=$(echo "$res1" | grep "SMART" | awk '{print $3}')
    bin1=$(echo "$res1" | grep "BINARY" | awk '{print $3}')

    res2=$($CASE2 <<< "$n")

    smart2=$(echo "$res2" | grep "SMART" | awk '{print $3}')
    bin2=$(echo "$res2" | grep "BINARY" | awk '{print $3}')

    echo "$n,$smart1,$bin1" >> $OUT1
    echo "$n,$smart2,$bin2" >> $OUT2

done

echo "DONE"