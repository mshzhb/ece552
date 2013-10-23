#!/bin/bash
for i in 2 4 8
do
  for j in 2 4 8
  do
    for k in gcc go compress
    do
      if [ ! -f "s22_${k}_${i}_${j}.sim" ]
      then
        sim-outorder -decode:width $i -issue:width $i -commit:width $i -issue:inorder true -res:ialu $j -redir:sim "s22_${k}_${i}_${j}.sim" -redir:prog "s22_${k}_${i}_${j}.out" "${k}.eio"
      fi
    done
  done
done
