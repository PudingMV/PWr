#!/bin/bash

if [ -z "$1" ]; then
  echo "Użycie: ./run_tests.sh k"
  exit 1
fi

k=$1

OUT="results.csv"
echo "n,algo,c,s" > $OUT

BIG=(1000 2000 3000 4000 5000 10000 20000 30000 40000 50000)

echo "Running tests..."

for n in "${BIG[@]}"; do
  for ((i=0;i<k;i++)); do

    ./gen_random $n | ./sort 0 | awk '{print $1",""merge"","$2","$3}' >> $OUT
    ./gen_random $n | ./sort 1 | awk '{print $1",""adaptive"","$2","$3}' >> $OUT

  done
done

echo "DONE -> results.csv"