#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>



int main(void) {
/*
	char buf[10];
	memset(buf, 'b', 10);
	char command[30];
	memset(command, 'c', 30);
	char greeting_text[5];
	memset(greeting_text, 'a', 5);
*/

//	strcpy(greeting_text, "Bonjour");
//	strcat(greeting_text, "Hello");


	char buf[256] = {0};
	memset(buf, 'b', sizeof(buf));
	char command[] = "uptime | sed 's/.*up \[^,]*\, .*/\\1/'";
	memset(command, 'c', sizeof(command));
	char greeting_text[128];
	memset(greeting_text, 'a', sizeof(greeting_text));
	
	char *ptr = greeting_text;

	for (int k = -1000; k < 1000; k++) {
		
		//printf("%d: %c\n", k, *(ptr + k));
		printf("%c ", *(ptr + k));
		
	}
	printf("\n\n");
	
//	strcpy(greeting_text, "Hello, dear ");
//	strcat(greeting_text, buf);

}







