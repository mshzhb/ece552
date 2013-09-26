#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, max;
	int temp = 0, sum = 0;
	int temp2 = 0, sum2 = 0;
	int dummy = 0; //for independent insn

	if ( argc != 2) {
		printf("Usage: %s <count>\n", argv[0]);
		exit(5);
	}

	max = atoi(argv[1]);

    /*
	Explanation
    (Assembly code from -O2 optimization level)
    Run this with sim-safe mbq1_opt2 (# of loops)
    
    I added no-ops before and after my loop
    to confirm that I am indeed looking at my loop.
    
    Notice that 4 RAW hazards exist inside this loop.
    Therefore the number of RAW hazards should be
    approximately equal to 4 * num of loop iterations.
    
	 #APP
		nop
	 #NO_APP
		li	$3,0x00000001		# 1
		blez	$4,$L16
	$L18:
		addu	$17,$17,3    temp = temp + 3
		addu	$20,$17,3    sum = temp + 3              <-- DATA HAZARD (OURS, SINGLE CYCLE)
		addu	$18,$18,5    temp2 temp2 + 5
		addu	$19,$19,1    dummy = dummy + 1
		addu	$21,$18,5    sum2 = temp2 + 5            <-- DATA HAZARD (OURS, DOUBLE CYCLE)
		addu	$3,$3,1     
		slt	$2,$4,$3         bit for branch decision     <-- DATA HAZARD (INHERENT, DOUBLE CYCLE)
		beq	$2,$0,$L18       check bit, loop or not loop <-- DATA HAZARD (INHERENT, DOUBLE CYCLE)
	$L16:
	 #APP
		nop

	As seen in my -O2 optimization, there are
    2 inherent data hazards created by the looping itself.
    */
    
	asm("nop");

	for(i = 1; i <= max; i++) {
		// the below code creates our
		// double cycle stall
		temp = temp + 3;
		sum = temp + 3;
		
		// the below code creates our
		// single cycle stall
		temp2 = temp2 + 5;
		dummy = dummy + 1;
		sum2 = temp2 + 5;		
	}

	asm("nop");
	
	printf("temp = %d\n",temp);
	printf("sum = %d\n",sum);
		
	printf("temp2 = %d\n",temp2);
	printf("sum2 = %d\n",sum2);

	printf("dummy = %d\n",dummy);

	return 0;
}
