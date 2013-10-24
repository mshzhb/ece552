#!/bin/bash
for prog in gcc go compress
do
  for width in 2 4
  do
    for latency in 2 4 6 8 10
    do
      if [ ! -f "s23_${prog}_nottaken_${width}_${latency}.sim" ]
      then
        ../sim-outorder -decode:width $width -issue:width $width -commit:width $width -issue:inorder true -fetch:mplat $latency -bpred nottaken -redir:sim "s23_${prog}_nottaken_${width}_${latency}.sim" -redir:prog "s23_${prog}_nottaken_${width}_${latency}.sim" ../$prog.eio
      fi
      if [ ! -f "s23_${prog}_perfect_${width}_${latency}.sim" ]
      then
        ../sim-outorder -decode:width $width -issue:width $width -commit:width $width -issue:inorder true -fetch:mplat $latency -bpred perfect -redir:sim "s23_${prog}_perfect_${width}_${latency}.sim" -redir:prog "s23_${prog}_perfect_${width}_${latency}.sim" ../$prog.eio
      fi
    done
  done
done
grep "sim_CPI" *.sim > ss23_static.sim
