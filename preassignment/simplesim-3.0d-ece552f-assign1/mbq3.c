#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, max, temp = 0, *x;
	*x = 5;

	if ( argc != 2) {
		printf("Usage: %s <count>\n", argv[0]);
		exit(5);
	}

	max = atoi(argv[1]);

/*
   #APP
      nop
   #NO_APP
      li  $2,0x00000001       # 1
      sw  $2,16($fp)
  $L3:
      lw  $2,16($fp)
      lw  $3,20($fp)
      slt $2,$3,$2
      beq $2,$0,$L6
      j   $L4
  $L6:
      lw  $2,28($fp)
      lw  $3,0($2)
      sw  $3,24($fp)
      lw  $2,16($fp)
      lw  $3,16($fp)
      addu    $2,$2,$3
      sw  $2,24($fp)
  $L5:
      lw  $3,16($fp)
      addu    $2,$3,1
      move    $3,$2
      sw  $3,16($fp)
      j   $L3
  $L4:
   #APP
      nop
   #NO_APP
*/
	asm("nop");

	// Two stalls required
	for(i = 1; i <= max; i++) {
		temp = *x;
		temp = i + i;
	}

	asm("nop");
	
	return 0;
}
