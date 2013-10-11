	.file	1 "mb.c"

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
	.ascii	"%d %d %d\000"
	.text
	.align	2
	.globl	main

	.extern	stdin, 4
	.extern	stdout, 4

	.text

	.loc	1 4
	.ent	main
main:
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, extra= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,40
	sw	$16,16($sp)
	move	$16,$4
	sw	$20,32($sp)
	move	$20,$5
	sw	$31,36($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	jal	__main
	li	$2,0x00000002		# 2
	beq	$16,$2,$L14
	.set	noreorder
	lw	$5,0($20)
	.set	reorder
	la	$4,$LC0
	jal	printf
	li	$4,0x00000005		# 5
	jal	exit
$L14:
	.set	noreorder
	lw	$4,4($20)
	.set	reorder
	jal	atoi
	move	$9,$2
 #APP
	nop
 #NO_APP
	li	$8,0x00000001		# 1
	blez	$9,$L16
	li	$11,0x66660000		# 1717960704
	ori	$11,$11,0x6667
	li	$10,0x55550000		# 1431633920
	ori	$10,$10,0x5556
$L18:
	move	$19,$8
	move	$17,$19
	mult	$17,$11
	.set	noreorder
	mfhi	$3
	mflo	$2
	#nop
	.set	reorder
	move	$18,$19
	sra	$5,$17,31
	srl	$6,$3,0
	move	$7,$0
	sra	$3,$6,2
	subu	$3,$3,$5
	sll	$2,$3,2
	addu	$2,$2,$3
	sll	$2,$2,1
	bne	$17,$2,$L19
	move	$18,$0
$L19:
	mult	$17,$10
	.set	noreorder
	mfhi	$3
	mflo	$2
	#nop
	#nop
	.set	reorder
	srl	$2,$3,0
	move	$3,$0
	subu	$2,$2,$5
	sll	$4,$2,1
	addu	$4,$4,$2
	bne	$17,$4,$L20
	move	$19,$0
$L20:
	sra	$3,$6,1
	subu	$3,$3,$5
	sll	$2,$3,2
	addu	$2,$2,$3
	bne	$17,$2,$L21
	move	$17,$0
$L21:
	beq	$18,$0,$L17
	move	$18,$0
$L17:
	addu	$8,$8,1
	slt	$2,$9,$8
	beq	$2,$0,$L18
$L16:
 #APP
	nop
 #NO_APP
	la	$4,$LC1
	move	$5,$18
	move	$6,$19
	move	$7,$17
	jal	printf
	move	$2,$0
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	addu	$sp,$sp,40
	j	$31
	.end	main
