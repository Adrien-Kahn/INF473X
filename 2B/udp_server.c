#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>


int main(int argc, char* argv[]) {
	
	// Fetching port number
	if (argc < 2) {
		fprintf(stderr, "Missing argument, please enter IP address and port number\n");
		return 1;
	}
	int port = atoi(argv[1]);
		
	// Building socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	while (1) {
	
	// Building addr_in for the server
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	for (int k = 0; k < 8; k++) myaddr.sin_zero[k] = 0;
	
	// Binding socket
	int bind_status = bind(sockfd, (struct sockaddr*) &myaddr, sizeof(struct sockaddr));
	if (bind_status != 0) printf("Failed to bind socket");
	
	// Building addr_in to identify the client
	struct sockaddr_in from;
	int *from_len;
	int temp = (int) sizeof(struct sockaddr);
	from_len = &temp;
	
	// Building received message buffer
	int buf_len = 1000;
	char buffer[buf_len];
	char *buf = buffer;
	
	
	// Receiving message
	int recv_len = recvfrom(sockfd, buf, buf_len, 0, (struct sockaddr*) &from, from_len);
	printf("Length of the received message: %d\n", recv_len);
	printf("Message content: \n%s\n", buf);

	// Sending it back
	int sent_length = sendto(sockfd, buf, recv_len, 0, (struct sockaddr*) &from, sizeof(struct sockaddr));
	printf("Sent length: %d\n", sent_length);
	if (sent_length != recv_len) printf("Message not sent entirely\n\n");
	}
}
