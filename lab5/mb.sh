#!/bin/sh

printf "" > mb_results.txt

for mb in mbq1 #mbq2 mbq6
do
  echo "Compiling ${mb}"
  # Compile microbenchmark
  /cad2/ece552f/compiler/bin/ssbig-na-sstrix-gcc $mb.c -O2 -o $mb

  # Generate intermediate assembly
  #/cad2/ece552f/compiler/bin/ssbig-na-sstrix-gcc $mb.c -O2 -S

  # Disassemble code
  #/cad2/ece552f/compiler/bin/ssbig-na-sstrix-objdump -x -d -l $mb

  for prefetch_type in nextline stride open
  do
    echo "Simulating ${prefetch_type} for ${mb}..."
    ./sim-cache -config cache-config/cache-lru-$prefetch_type.cfg -redir:sim $mb.sim -redir:prog trash.txt $mb

    echo "${mb} ${prefetch_type} results" >> mb_results.txt
    echo "====================" >> mb_results.txt
    echo "" >> mb_results.txt

    grep dl1.accesses $mb.sim >> mb_results.txt
    grep dl1.hits $mb.sim >> mb_results.txt
    grep dl1.misses $mb.sim >> mb_results.txt
    grep dl1.replacements $mb.sim >> mb_results.txt
    grep dl1.miss_rate $mb.sim >> mb_results.txt
    grep dl1.repl_rate $mb.sim >> mb_results.txt
    grep dl1.wb_rate $mb.sim >> mb_results.txt
    grep dl1.inv_rate $mb.sim >> mb_results.txt
    grep dl1.prefetch_accesses $mb.sim >> mb_results.txt
    grep dl1.prefetch_hits $mb.sim >> mb_results.txt
    grep dl1.prefetch_misses $mb.sim >> mb_results.txt

    echo "" >> mb_results.txt
  done
done
