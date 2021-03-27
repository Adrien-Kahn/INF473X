#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "itest.h"


// Returns segmentation fault


int main(void) {
	struct addrinfo *hints;
	memset(hints, 0, sizeof(*hints));
	printf("%d\n", hints->ai_flags);
	return 0;
}



// Returns expected value (0)

/*

int main(void) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	printf("%d\n", hints.ai_flags);
	return 0;
}

*/
