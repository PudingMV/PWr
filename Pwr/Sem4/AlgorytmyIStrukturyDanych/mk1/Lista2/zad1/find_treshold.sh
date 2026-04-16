#!/bin/bash

k=10
OUT="threshold_results.txt"

> $OUT

for t in 5 10 15 20 30 40 50
do
  echo "Testing threshold = $t"

  total_c=0
  total_s=0

  for n in $(seq 1000 1000 10000)
  do
    for ((i=0;i<k;i++))
    do
      result=$(./gen_random $n | ./hybrid 0 $t)

      c=$(echo $result | awk '{print $2}')
      s=$(echo $result | awk '{print $3}')

      total_c=$((total_c + c))
      total_s=$((total_s + s))
    done
  done

  echo "$t $total_c $total_s" >> $OUT
done