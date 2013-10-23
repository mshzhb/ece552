sim-outorder -decode:width 1 -issue:width 1 -commit:width 1 -issue:inorder true -redir:sim gcc1.sim -redir:prog gcc1.out gcc.eio
sim-outorder -decode:width 1 -issue:width 1 -commit:width 1 -issue:inorder true -redir:sim go1.sim -redir:prog go1.out go.eio
sim-outorder -decode:width 1 -issue:width 1 -commit:width 1 -issue:inorder true -redir:sim compress1.sim -redir:prog compress1.out compress.eio
sim-outorder -decode:width 2 -issue:width 2 -commit:width 2 -issue:inorder true -redir:sim gcc2.sim -redir:prog gcc2.out gcc.eio
sim-outorder -decode:width 2 -issue:width 2 -commit:width 2 -issue:inorder true -redir:sim go2.sim -redir:prog go2.out go.eio
sim-outorder -decode:width 2 -issue:width 2 -commit:width 2 -issue:inorder true -redir:sim compress2.sim -redir:prog compress2.out compress.eio
sim-outorder -decode:width 4 -issue:width 4 -commit:width 4 -issue:inorder true -redir:sim gcc4.sim -redir:prog gcc4.out gcc.eio
sim-outorder -decode:width 4 -issue:width 4 -commit:width 4 -issue:inorder true -redir:sim go4.sim -redir:prog go4.out go.eio
sim-outorder -decode:width 4 -issue:width 4 -commit:width 4 -issue:inorder true -redir:sim compress4.sim -redir:prog compress4.out compress.eio
sim-outorder -decode:width 8 -issue:width 8 -commit:width 8 -issue:inorder true -redir:sim gcc8.sim -redir:prog gcc8.out gcc.eio
sim-outorder -decode:width 8 -issue:width 8 -commit:width 8 -issue:inorder true -redir:sim go8.sim -redir:prog go8.out go.eio
sim-outorder -decode:width 8 -issue:width 8 -commit:width 8 -issue:inorder true -redir:sim compress8.sim -redir:prog compress8.out compress.eio

grep "sim_CPI" *.sim
