#!/bin/sh

for mb in mbq1 #mbq2 mbq6
do
  echo "Compiling ${mb}"
  # Compile microbenchmark
  /cad2/ece552f/compiler/bin/ssbig-na-sstrix-gcc $mb.c -O2 -o $mb

  # Generate intermediate assembly
  /cad2/ece552f/compiler/bin/ssbig-na-sstrix-gcc $mb.c -O2 -S

  # Disassemble code
  #/cad2/ece552f/compiler/bin/ssbig-na-sstrix-objdump -x -d -l $mb
done
