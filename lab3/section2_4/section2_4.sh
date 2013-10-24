#!/bin/bash
for i in 2 4 8
do
  for j in 2 4 8
  do
    for k in gcc go compress
    do
      if [ ! -f "s24_${k}_${i}_${j}.sim" ]
      then
        ../sim-outorder -fetch:ifqsize $i -decode:width $i -issue:width $i -commit:width $i -issue:inorder true -lsq:size $j -redir:sim "s24_${k}_${i}_${j}.sim" -redir:prog "s24_${k}_${i}_${j}.out" "../${k}.eio"

      fi
    done
  done
done

grep "sim_CPI" s24_*.sim >> section2_4_CPIs.sim

