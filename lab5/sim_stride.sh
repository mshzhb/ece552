#!/bin/sh

rm stride_results.txt
touch stride_results.txt

for size in 32 64 128 256 512 1024 2048 4096 16384 65536
do
  echo "********************" >> stride_results.txt
  echo "stride size ${size}" >> stride_results.txt
  echo "********************" >> stride_results.txt
  echo "" >> stride_results.txt

  for prog in compress #gcc go compress
  do
    echo "Simulating ${prog} for size ${size}..."
    ./sim-cache -config cache-config/cache-lru-stride-$size.cfg -redir:sim $prog.sim -redir:prog trash.txt /cad2/ece552f/benchmarks/$prog.eio

    echo "${prog} ${size} results" >> stride_results.txt
    grep dl1.miss_rate $prog.sim >> stride_results.txt

    echo "" >> stride_results.txt
  done
done

rm trash.txt
