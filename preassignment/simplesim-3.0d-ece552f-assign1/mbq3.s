	.file	1 "mbq3.c"

 # GNU C 2.7.2.3 [AL 1.1, MM 40, tma 0.1] SimpleScalar running sstrix compiled by GNU C

 # Cc1 defaults:
 # -mgas -mgpOPT

 # Cc1 arguments (-G value = 8, Cpu = default, ISA = 1):
 # -quiet -dumpbase -O0 -o

gcc2_compiled.:
__gnu_compiled_c:
	.rdata
	.align	2
$LC0:
	.ascii	"Usage: %s <count>\n\000"
	.text
	.align	2
	.globl	main

	.text

	.loc	1 4
	.ent	main
main:
	.frame	$fp,40,$31		# vars= 16, regs= 2/0, args= 16, extra= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,40
	sw	$31,36($sp)
	sw	$fp,32($sp)
	move	$fp,$sp
	sw	$4,40($fp)
	sw	$5,44($fp)
	jal	__main
	sw	$0,24($fp)
	lw	$2,28($fp)
	li	$3,0x00000005		# 5
	sw	$3,0($2)
	lw	$2,40($fp)
	li	$3,0x00000002		# 2
	beq	$2,$3,$L2
	lw	$2,44($fp)
	la	$4,$LC0
	lw	$5,0($2)
	jal	printf
	li	$4,0x00000005		# 5
	jal	exit
$L2:
	lw	$3,44($fp)
	addu	$2,$3,4
	lw	$4,0($2)
	jal	atoi
	sw	$2,20($fp)
 #APP
	nop
 #NO_APP
	li	$2,0x00000001		# 1
	sw	$2,16($fp)
$L3:
	lw	$2,16($fp)
	lw	$3,20($fp)
	slt	$2,$3,$2
	beq	$2,$0,$L6
	j	$L4
$L6:
	lw	$2,28($fp)
	lw	$3,0($2)
	sw	$3,24($fp)
	lw	$2,16($fp)
	lw	$3,16($fp)
	addu	$2,$2,$3
	sw	$2,24($fp)
$L5:
	lw	$3,16($fp)
	addu	$2,$3,1
	move	$3,$2
	sw	$3,16($fp)
	j	$L3
$L4:
 #APP
	nop
 #NO_APP
	move	$2,$0
	j	$L1
$L1:
	move	$sp,$fp			# sp not trusted here
	lw	$31,36($sp)
	lw	$fp,32($sp)
	addu	$sp,$sp,40
	j	$31
	.end	main
