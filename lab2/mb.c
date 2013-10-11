#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, max;
	int a, b, c;

	if ( argc != 2) {
		printf("Usage: %s <count>\n", argv[0]);
		exit(5);
	}

	max = atoi(argv[1]);
	/*
	 #APP
		nop
	 #NO_APP
		li	$8,0x00000001		# 1
		blez	$9,$L16				INIT BRANCH CONDITION
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
		bne	$17,$2,$L19				BRANCH 1: a%10 == 0
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
		bne	$17,$4,$L20				BRANCH 2: b%3 == 0
		move	$19,$0
	$L20:
		sra	$3,$6,1
		subu	$3,$3,$5
		sll	$2,$3,2
		addu	$2,$2,$3
		bne	$17,$2,$L21				BRANCH 3: c%5 == 0
		move	$17,$0
	$L21:
		beq	$18,$0,$L17				BRANCH 4: a != 0
		move	$18,$0
	$L17:
		addu	$8,$8,1
		slt	$2,$9,$8
		beq	$2,$0,$L18				BRANCH 5: LOOP BRANCH
	$L16:
	 #APP
		nop
	*/

	asm("nop");

	for(i = 1; i <= max; i++) {
		a = i, b = i, c = i;
		
		if(a%10 == 0) {
			a = 0;
		}
		if(b%3 == 0) {
			b = 0;
		}
		
		if(c%5 == 0) {
			c = 0;
		}
		
		if(a != 0) {
			a = 0;
		}
	}

	asm("nop");
	printf("%d %d %d",a,b,c);
	return 0;
}

