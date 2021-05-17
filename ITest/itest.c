#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include "itest.h"


// Returns segmentation fault

/*

int main(void) {
	struct addrinfo *hints;
	memset(hints, 0, sizeof(*hints));
	printf("%d\n", hints->ai_flags);
	return 0;
}

*/

// Returns expected value (0)

/*

int main(void) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	printf("%d\n", hints.ai_flags);
	return 0;
}

*/


/*
int main(void) {
	char *s;
	s = (char *) malloc(5);
	
	printf("1\n");
	
	strcat(s, "ayo salut coucou comment ca va");
	
	printf("1\n");
	
	s = (char *) realloc(s, 100);
	
	printf("1\n");
	
	printf("%s\n", s);
		
	
}
*/

#include <sys/mman.h>

unsigned char code[] = \
"\xeb\x19\x31\xc0\x31\xdb\x31\xd2\x31\xc9\xb0\x04\xb3\x01\x59\xb2\x05\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe2\xff\xff\xff\x68\x65\x6c\x6c\x6f";

int main(){
  printf("Shellcode length: %d\n", strlen(code));
  int r =  mprotect((void *)((int)code & ~4095),  4096, PROT_READ | PROT_WRITE|PROT_EXEC);
  printf("mprotect: %d\n",r);
  int (*ret)() = (int(*)())code;
  return ret();
}



