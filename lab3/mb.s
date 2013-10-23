	.file	1 "mb.c"

 # GNU C 2.7.2.3 [AL 1.1, MM 40, tma 0.1] SimpleScalar running sstrix compiled by GNU C

 # Cc1 defaults:
 # -mgas -mgpOPT

 # Cc1 arguments (-G value = 8, Cpu = default, ISA = 1):
 # -quiet -dumpbase -funroll-all-loops -o

gcc2_compiled.:
__gnu_compiled_c:
	.rdata
	.align	2
$LC0:
	.ascii	"Usage: %s <count>\n\000"
	.align	2
$LC1:
	.ascii	"%d %d %d %d\000"
	.text
	.align	2
	.globl	main

	.text

	.loc	1 4
	.ent	main
main:
	.frame	$fp,56,$31		# vars= 24, regs= 2/0, args= 24, extra= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,56
	sw	$31,52($sp)
	sw	$fp,48($sp)
	move	$fp,$sp
	sw	$4,56($fp)
	sw	$5,60($fp)
	jal	__main
	lw	$2,56($fp)
	li	$3,0x00000002		# 2
	beq	$2,$3,$L2
	lw	$2,60($fp)
	la	$4,$LC0
	lw	$5,0($2)
	jal	printf
	li	$4,0x00000005		# 5
	jal	exit
$L2:
	li	$2,0x000f4240		# 1000000
	sw	$2,28($fp)
 #APP
	nop
 #NO_APP
	li	$2,0x00000001		# 1
	sw	$2,24($fp)
$L3:
	lw	$2,24($fp)
	lw	$3,28($fp)
	slt	$2,$3,$2
	beq	$2,$0,$L6
	j	$L4
$L6:
 #APP
	addi $13, $12, 1
	addi $15, $14, 1
	addi $17, $16, 1
	addi $19, $18, 1
 #NO_APP
$L5:
	lw	$3,24($fp)
	addu	$2,$3,1
	move	$3,$2
	sw	$3,24($fp)
	j	$L3
$L4:
 #APP
	nop
 #NO_APP
	lw	$2,44($fp)
	sw	$2,16($sp)
	la	$4,$LC1
	lw	$5,32($fp)
	lw	$6,36($fp)
	lw	$7,40($fp)
	jal	printf
	move	$2,$0
	j	$L1
$L1:
	move	$sp,$fp			# sp not trusted here
	lw	$31,52($sp)
	lw	$fp,48($sp)
	addu	$sp,$sp,56
	j	$31
	.end	main
