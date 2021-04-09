#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>


int main(int argc, char* argv[]) {
	
	int msg_max_len = 1000;
	
	if (argc < 3) {
		fprintf(stderr, "Missing argument, please enter IP address and port number\n");
		return 1;
	}
	
	char *ip = argv[1];
	int port = atoi(argv[2]);
	
	while (1) {
	
	// Getting message from user
	char message[msg_max_len];
	char *msg = message;
	printf("Please input a message to send:\n--> ");
	fgets(message, msg_max_len, stdin);
	int msg_len = strlen(msg);
	printf("Message length: %d\n", msg_len);

	// Building socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	// Building destination addr_in
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &(dest.sin_addr)) != 1) printf("inet_pton error\n\n");
	for (int k = 0; k < 8; k++) dest.sin_zero[k] = 0;
	
	
	// Building addr_in for the client
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	for (int k = 0; k < 8; k++) myaddr.sin_zero[k] = 0;
	
	// Binding socket
	//int bind_status = bind(sockfd, (struct sockaddr*) &myaddr, sizeof(struct sockaddr));
	//if (bind_status != 0) printf("Failed to bind socket\n");
	
	// Building addr_in to identify the replying server
	struct sockaddr_in from;
	int *from_len;
	int temp = (int) sizeof(struct sockaddr);
	from_len = &temp;
	

	
	
	// Sending the message
	int sent_length = sendto(sockfd, msg, msg_len, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr));
	printf("Sent length: %d\n\n\n", sent_length);
	if (sent_length != msg_len) printf("Message not sent entirely\n\n");
	
	// Building received message buffer
	int buf_len = 1000;
	char buffer[buf_len];
	char *buf = buffer;
	memset(buf, 0, 1000);
	
	// Receiving message
	int recv_len = recvfrom(sockfd, buf, buf_len, 0, (struct sockaddr*) &from, from_len);
	printf("Length of the received message: %d\n", recv_len);
	printf("Message content: \n%s\n", buf);
	
	}
	
	return 0;
}
