#!/bin/sh

rm stride_results.txt
touch stride_results.txt

for size in 64 256 1024 4096 16384 65536
do
  echo "********************" >> stride_results.txt
  echo "stride size ${size}" >> stride_results.txt
  echo "********************" >> stride_results.txt
  echo "" >> stride_results.txt

  for prog in gcc go compress
  do
    echo "Simulating ${prog} for size ${size}..."
    ./sim-cache -config cache-config/cache-lru-stride-$size.cfg -redir:sim $prog.sim -redir:prog trash.txt /cad2/ece552f/benchmarks/$prog.eio

    echo "${prog} results" >> stride_results.txt
    echo "====================" >> stride_results.txt
    echo "" >> stride_results.txt

    grep dl1.miss_rate $prog.sim >> stride_results.txt
    grep dl1.repl_rate $prog.sim >> stride_results.txt
    grep dl1.wb_rate $prog.sim >> stride_results.txt
    grep dl1.inv_rate $prog.sim >> stride_results.txt
    grep dl1.prefetch_accesses $prog.sim >> stride_results.txt
    grep dl1.prefetch_hits $prog.sim >> stride_results.txt
    grep dl1.prefetch_misses $prog.sim >> stride_results.txt

    echo "" >> stride_results.txt
  done
done

rm trash.txt
