sbig-na-sstrix-gcc mb.c -funroll-loops -O2 -o mb
sbig-na-sstrix-gcc mb.c -funroll-loops -O2 -S
sim-outorder -fetch:ifqsize 8 -decode:width 8 -issue:width 8 -commit:width 8 -issue:inorder true mb &
