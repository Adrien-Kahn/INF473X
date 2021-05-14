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

	char buf[10];
	memset(buf, 'b', 10);
	char command[] = "Hello";
	char greeting_text[20];
	memset(greeting_text, 'a', 20);
//	strcpy(greeting_text, "Bonjour");
//	strcat(greeting_text, buf);
	
	char *ptr = greeting_text;

	for (int k = -50; k < 50; k++) {
		
		printf("%d: %c\n", k, *(ptr + k));
	
	}
	
//	strcpy(greeting_text, "Hello, dear ");
//	strcat(greeting_text, buf);

}







