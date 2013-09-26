	.file	1 "mbq1.c"

 # GNU C 2.7.2.3 [AL 1.1, MM 40, tma 0.1] SimpleScalar running sstrix compiled by GNU C

 # Cc1 defaults:
 # -mgas -mgpOPT

 # Cc1 arguments (-G value = 8, Cpu = default, ISA = 1):
 # -quiet -dumpbase -O2 -o

gcc2_compiled.:
__gnu_compiled_c:
	.rdata
	.align	2
$LC0:
	.ascii	"Usage: %s <count>\n\000"
	.align	2
$LC1:
	.ascii	"temp = %d\n\000"
	.align	2
$LC2:
	.ascii	"sum = %d\n\000"
	.align	2
$LC3:
	.ascii	"temp2 = %d\n\000"
	.align	2
$LC4:
	.ascii	"sum2 = %d\n\000"
	.align	2
$LC5:
	.ascii	"dummy = %d\n\000"
	.text
	.align	2
	.globl	main

	.extern	stdin, 4
	.extern	stdout, 4

	.text

	.loc	1 4
	.ent	main
main:
	.frame	$sp,48,$31		# vars= 0, regs= 8/0, args= 16, extra= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,48
	sw	$16,16($sp)
	move	$16,$4
	sw	$22,40($sp)
	move	$22,$5
	sw	$31,44($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	jal	__main
	move	$17,$0
	move	$20,$0
	move	$18,$0
	move	$21,$0
	move	$19,$0
	li	$2,0x00000002		# 2
	beq	$16,$2,$L14
	.set	noreorder
	lw	$5,0($22)
	.set	reorder
	la	$4,$LC0
	jal	printf
	li	$4,0x00000005		# 5
	jal	exit
$L14:
	.set	noreorder
	lw	$4,4($22)
	.set	reorder
	jal	atoi
	move	$4,$2
 #APP
	nop
 #NO_APP
	li	$3,0x00000001		# 1
	blez	$4,$L16
$L18:
	addu	$17,$17,3
	addu	$20,$17,3
	addu	$18,$18,5
	addu	$19,$19,1
	addu	$21,$18,5
	addu	$3,$3,1
	slt	$2,$4,$3
	beq	$2,$0,$L18
$L16:
 #APP
	nop
 #NO_APP
	la	$4,$LC1
	move	$5,$17
	jal	printf
	la	$4,$LC2
	move	$5,$20
	jal	printf
	la	$4,$LC3
	move	$5,$18
	jal	printf
	la	$4,$LC4
	move	$5,$21
	jal	printf
	la	$4,$LC5
	move	$5,$19
	jal	printf
	move	$2,$0
	lw	$31,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	addu	$sp,$sp,48
	j	$31
	.end	main
