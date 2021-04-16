#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define N_THREAD 2

void *process_message(void *);

typedef struct Recv_info{
	struct sockaddr_in from;
	int *from_len;
	int buf_len;
	char *buf;
} Recv_info;


int main(int argc, char* argv[]) {
	
	// Fetching port number
	if (argc < 2) {
		fprintf(stderr, "Missing argument, please enter IP address and port number\n");
		return 1;
	}
	int port = atoi(argv[1]);
		
	// Building socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	// Building addr_in for the server
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	for (int k = 0; k < 8; k++) myaddr.sin_zero[k] = 0;
	
	// Binding socket
	int bind_status = bind(sockfd, (struct sockaddr*) &myaddr, sizeof(struct sockaddr));
	if (bind_status != 0) printf("Failed to bind socket");


	// Creating threads that run process_message
	for (int k = 0; k < N_THREAD; k++) {
		pthread_t thread;
		if (pthread_create(&thread, NULL, process_message, &sockfd)) {
			fprintf(stderr, "Failed to create thread\n");
			return 1;
		}
	}

	// Main threads does nothing
	while (1) usleep(100000000);
	
}


	
void *process_message(void *sock_pointer) {

	int sockfd = *((int *)sock_pointer);
	
	// Building addr_in to identify the client
	struct sockaddr_in from;
	int *from_len;
	int temp = (int) sizeof(struct sockaddr);
	from_len = &temp;
	
	// Building received message buffer
	int buf_len = 1000;
	char buffer[buf_len];
	char *buf = buffer;
	memset(buf, 0, 1000);
	
	// Receiving message
	int recv_len = recvfrom(sockfd, buf, buf_len, 0, (struct sockaddr*) &from, from_len);
	printf("Length of the received message: %d\n", recv_len);
	printf("Message content: \n%s\n", buf);
	
	// Sleeping because receiving a message is tiring
	usleep(10000000);
	
	// Sending it back
	int sent_length = sendto(sockfd, buf, recv_len, 0, (struct sockaddr*) &from, sizeof(struct sockaddr));
	printf("Sent length: %d\n", sent_length);
	if (sent_length != recv_len) printf("Message not sent entirely\n\n");
	
	return NULL;

}
