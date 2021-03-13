#include <stdio.h>
#include <string.h>
#include "fancy-hello-world.h"

int main(void) {

	char output[200];
		
	char name[100];
	printf("Please, input your name: ");
	fgets(name, 100, stdin);
	
	hello_string(name, output);
	
	printf("%s", output);

	return 0;
}

void hello_string(char* name, char* output) {
	char c[100] = "Hello World, hello ";
	strcat(c, name);
	strcpy(output, c);
}
