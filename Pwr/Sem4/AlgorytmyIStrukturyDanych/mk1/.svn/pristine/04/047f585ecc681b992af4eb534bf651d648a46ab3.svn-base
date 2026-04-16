#!/bin/bash

if [ -z "$1" ]; then
  echo "Użycie: ./run_tests.sh k"
  exit 1
fi

k=$1

OUT="results.csv"
echo "mn,m,n,found,comparisons" > $OUT

SIZES=(
  "10 10"
  "20 20"
  "30 30"
  "40 40"
  "50 50"
  "100 100"
  "200 200"
  "300 300"
  "400 400"
  "500 500"
)

echo "Running tests..."

for size in "${SIZES[@]}"; do
    m=$(echo $size | cut -d' ' -f1)
    n=$(echo $size | cut -d' ' -f2)

    for ((i=0;i<k;i++)); do

        x=$((RANDOM % (m*n)))

        ./gen_matrix $m $n | ./search $x | \
        awk -v mn=$((m*n)) -v m=$m -v n=$n '{print mn","m","n","$1","$3}' >> $OUT

    done
done

echo "DONE -> results.csv"