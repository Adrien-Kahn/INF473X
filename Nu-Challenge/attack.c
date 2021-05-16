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
	
	char msg[256];
	char *ptr = msg;
	memset(msg, 0, 256);
	int n = atoi(argv[2]);
	for (int k = 0; k < n; k++) {
		*(ptr + k) = 'A';
	}
	*(ptr + n) = '\n';
	
	printf("msg: %s\n\n", msg);
	
	// Building destination addr_in
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[1]));
	if (inet_pton(AF_INET, "127.0.0.1", &(dest.sin_addr)) != 1) printf("inet_pton error\n\n");
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
	} else {
		printf("Connected successfully\n\n");
	}

	
	// Message 1
	int buf_len = 2000;
	char recvbuf[buf_len];
	memset(recvbuf, 0, buf_len);
	if (recv(sockfd, recvbuf, buf_len, 0) == -1) {
		printf("Error receiving message");
  		exit(1);
	}
	printf("Message 1: %s\n", recvbuf);
	
	
	// Sending name
	if (send(sockfd, msg, strlen(msg), 0) != strlen(msg)) {
		printf("Failed to send message entirely");
  		exit(1);
	} else {
		printf("Message sent\n\n");
	}
	
	// Reply
	memset(recvbuf, 0, buf_len);
	if (recv(sockfd, recvbuf, buf_len, 0) == -1) {
		printf("Error receiving message");
  		exit(1);
	}
	printf("%s\n", recvbuf);
	
	return 0;
	
}
