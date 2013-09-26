#!/bin/bash

#this runs sim-safe with verbose and gcc.eio
#can specify num instructions, else it defaults

clear

if [ -z "$1" ]
	then sim-safe -max:inst 10 -v /cad2/ece552f/benchmarks/gcc.eio
else
	sim-safe -max:inst $1 -v /cad2/ece552f/benchmarks/gcc.eio
fi
