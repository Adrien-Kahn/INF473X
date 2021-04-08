#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include"udp_client.h"


int main(int argc, char* argv[]) {
	
	int msg_max_len = 1000;
	
	if (argc < 3) {
		fprintf(stderr, "Missing argument, please enter IP address and port number\n");
		return 1;
	}
	
	char *ip = argv[1];
	int port = atoi(argv[2]);
	
	// Getting message from user
	char message[msg_max_len];
	char *msg = message;
	printf("Please input a message to send:\n--> ");
	fgets(message, msg_max_len, stdin);
	
	// Building socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	// Building addr_in
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &(dest.sin_addr)) != 0) printf("inet_pton error\n\n");
	for (int k = 0; k < 8; k++) dest.sin_zero[k] = 0;
	
	return 0;
}
