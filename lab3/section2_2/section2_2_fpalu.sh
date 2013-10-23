#!/bin/bash
for j in 2 4 8
do
  for k in gcc go compress
  do
    if [ ! -f "s22_${k}_4_${j}_fpalu.sim" ]
    then
      sim-outorder -decode:width 4 -issue:width 4 -commit:width 4 -issue:inorder true -res:fpalu $j -redir:sim "s22_${k}_4_${j}_fpalu.sim" -redir:prog "s22_${k}_4_${j}_fpalu.out" "${k}.eio"
    fi
  done
done
