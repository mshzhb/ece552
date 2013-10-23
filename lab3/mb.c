#include <stdio.h>

int main(int argc, char *argv[])
{
  int i = 0;
	asm("nop");
	for(i = 1; i <= 1000000; i++) {
    asm("move $12, $0");
    asm("move $13, $0");
    asm("move $14, $0");
    asm("move $15, $0");
    asm("move $16, $0");
    asm("move $17, $0");
    asm("move $18, $0");
    asm("move $19, $0");
	}
	asm("nop");
	return 0;
}

