#include <stdio.h>

int main(void) {
	
	// load to use hazard
	asm("lw $2, 0($4)");
	asm("addu $2, $2, 3");
	
	// load to use hazard bypass
	asm("lw $5, 0($4)");
	asm("addu $3, $3, 3");
	asm("addu $5, $5, 3");

	return 0;
}
