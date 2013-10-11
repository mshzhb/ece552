#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, max;

	if ( argc != 2) {
		printf("Usage: %s <count>\n", argv[0]);
		exit(5);
	}

	max = atoi(argv[1]);
	asm("nop");

	for(i = 1; i <= max; i++) {
		int a = i, b = i, c = i;

		if(a%2 == 0) {
			a = 0;
		}
		if(b%2 == 0) {
			b = 0;
		}
		if(a != 0) {
			a = 0;
			b = 0;
		}
		
		if(c%3 == 0) {
			c = 0;
		}
	}

	asm("nop");
	return 0;
}

