#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, max;
	int a, b, c, d;

	if ( argc != 2) {
		printf("Usage: %s <count>\n", argv[0]);
		exit(5);
	}

	//max = atoi(argv[1]);
  max = 1000000;
	asm("nop");

	for(i = 1; i <= max; i++) {
    asm("addi $13, $12, 1");
    asm("addi $15, $14, 1");
    asm("addi $17, $16, 1");
    asm("addi $19, $18, 1");
	}

	asm("nop");
	printf("%d %d %d %d",a,b,c,d);
	return 0;
}

