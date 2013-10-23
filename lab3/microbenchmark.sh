ssbig-na-sstrix-gcc mb.c -funroll-loops -O2 -o mb
ssbig-na-sstrix-gcc mb.c -funroll-loops -O2 -S
for i in 1 2 4 8
do
  sim-outorder -fetch:ifqsize $i -decode:width $i -issue:width $i -commit:width $i -issue:inorder true -redir:sim mb$i.sim mb
done
grep "sim_CPI" mb*.sim > mbCPIs.sim
