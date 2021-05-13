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


int main(void) {

	char msg[256] = "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111&1111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ls";
	printf("%s\n\n", msg);
	
	// Building destination addr_in
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_port = htons(1234);
	if (inet_pton(AF_INET, "127.0.0.1", &(dest.sin_addr)) != 1) printf("inet_pton error\n\n");
	for (int k = 0; k < 8; k++) dest.sin_zero[k] = 0;

	// Building the socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	// Connecting
	int status = connect(sockfd, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));
	printf("Connection status: %d\n", status);

	
	// Receiving stuff
	int buf_len = 2000;
	char recvbuf[buf_len];
	memset(recvbuf, 0, buf_len);
	recv(sockfd, recvbuf, buf_len, 0);
	printf("%s\n", recvbuf);
	
	// Sending name
	send(sockfd, msg, strlen(msg), 0);
	
	// Receiving more stuff
	memset(recvbuf, 0, buf_len);
	recv(sockfd, recvbuf, buf_len, 0);
	printf("%s\n", recvbuf);
	
	memset(recvbuf, 0, buf_len);
	recv(sockfd, recvbuf, buf_len, 0);
	printf("%s\n", recvbuf);
	
	return 0;
	
}
