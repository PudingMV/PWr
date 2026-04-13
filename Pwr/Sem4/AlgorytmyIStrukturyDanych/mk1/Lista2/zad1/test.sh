#!/bin/bash

if [ -z "$1" ]; then
  echo "Użycie: ./test.sh k"
  exit 1
fi

k=$1

mkdir -p results

INS="results/insertion_k${k}.txt"
QUI="results/quick_k${k}.txt"
HYB="results/hybrid_k${k}.txt"

> $INS
> $QUI
> $HYB

echo "Małe n (10–50)..."

for n in $(seq 10 10 50)
do
  for ((i=0;i<k;i++))
  do
    ./gen_random $n | ./insertion 0 >> $INS
    ./gen_random $n | ./quick 0 >> $QUI
    ./gen_random $n | ./hybrid 0 5 >> $HYB
  done
done

echo "Duże n (1000–50000)..."

for n in $(seq 1000 1000 50000)
do
  for ((i=0;i<k;i++))
  do
    ./gen_random $n | ./quick 0 >> $QUI
    ./gen_random $n | ./hybrid 0 5 >> $HYB
  done
done

echo "Zapisano do folderu results/"