#!/bin/bash
for prog in gcc go compress
do
  for width in 2 4
  do
    for config_param in 1 32 128
    do
      if [ ! -f "s23_${prog}_2lev_${width}_${config_param}.2levsim" ]
      then
        ../sim-outorder -decode:width $width -issue:width $width -commit:width $width -issue:inorder true -bpred 2lev -bpred:2lev $config_param 256 8 0 -redir:sim "s23_${prog}_2lev_${width}_${config_param}.2levsim" -redir:prog "s23_${prog}_2lev_${width}_${config_param}.2levout" ../$prog.eio
      fi
    done
  done
done
grep "sim_CPI" *.2levsim > ss23_dynamic.sim
