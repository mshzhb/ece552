#!/bin/sh

rm results.txt
touch results.txt

for prefetch_type in nextline stride #open
do
  echo "********************" >> results.txt
  echo "${prefetch_type} results" >> results.txt
  echo "********************" >> results.txt
  echo "" >> results.txt
  for prog in gcc go compress
  do
    echo "Simulating ${prefetch_type} ${prog}..."
    ./sim-cache -config cache-config/cache-lru-$prefetch_type.cfg -redir:sim $prog.sim -redir:prog trash.txt /cad2/ece552f/benchmarks/$prog.eio

    echo "${prog} results" >> results.txt
    echo "====================" >> results.txt
    echo "" >> results.txt
    grep dl1.miss_rate $prog.sim >> results.txt
    grep dl1.repl_rate $prog.sim >> results.txt
    grep dl1.wb_rate $prog.sim >> results.txt
    grep dl1.inv_rate $prog.sim >> results.txt
    grep dl1.prefetch_accesses $prog.sim >> results.txt
    grep dl1.prefetch_hits $prog.sim >> results.txt
    grep dl1.prefetch_misses $prog.sim >> results.txt
    echo "" >> results.txt
  done
done

rm trash.txt
