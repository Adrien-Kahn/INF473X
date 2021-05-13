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

int main(void) {

	char buf[256] = {0};
	printf("%d\n", sizeof(buf));
	return 0;

}
