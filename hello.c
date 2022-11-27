#include <stdio.h>

int MY_GLOBAL_VALUE = 7;

void say_stuff() {
	printf("Hello, world!\n");
}

int add1000() {
	say_stuff();
	int x = 0;
	for(int i = 0; i < 1000; i++) {
		x++;
	}
	return x;
}

int main(int argc, char** argv) {
	// printf("Hello World\n");
	// asm("int 3"); // Breakpoint in intel syntax (-masm=intel)
	int x = add1000();	
	return 0;
}