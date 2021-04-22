#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MSG_MAX_LEN 1000
#define MY_PORT 9000


void *send_message(void *);
void *receive_message(void *);

typedef struct send_info{
	struct sockaddr_in dest;
	int sock;
} send_info;


pthread_mutex_t writeMutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char* argv[]) {
	
	if (argc < 3) {
		fprintf(stderr, "Missing argument, please enter IP address and port number\n");
		return 1;
	}
	
	char *ip = argv[1];
	int port = atoi(argv[2]);

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
	myaddr.sin_port = htons(MY_PORT);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	for (int k = 0; k < 8; k++) myaddr.sin_zero[k] = 0;
	
	// Binding socket
	int bind_status = bind(sockfd, (struct sockaddr*) &myaddr, sizeof(struct sockaddr));
	if (bind_status != 0) printf("Failed to bind socket, maybe it is already bound\n");
	
	send_info sf;
	sf.dest = dest;
	sf.sock = sockfd;
	
	pthread_t send_thread;
	if (pthread_create(&send_thread, NULL, send_message, &sf)) {
		fprintf(stderr, "Failed to create thread\n");
		return 1;
	}
	
	pthread_t receive_thread;
	if (pthread_create(&receive_thread, NULL, receive_message, &sockfd)) {
		fprintf(stderr, "Failed to create thread\n");
		return 1;
	}
	
	// Main threads does nothing
	while (1) usleep(100000000);	
	
	return 0;
}



void *send_message(void *ptr) {
	
	send_info sf = *((send_info *) ptr);
	
	while (1) {
	
		usleep(100000);
		
		pthread_mutex_lock(&writeMutex);
		
		// Getting message from user
		char message[MSG_MAX_LEN];
		char *msg = message;
		printf("Please input a message to send:\n\n");
		fgets(message, MSG_MAX_LEN, stdin);
		int msg_len = strlen(msg);
		//printf("Message length: %d\n", msg_len);
		
		// Sending the message
		int sent_length = sendto(sf.sock, msg, msg_len, 0, (struct sockaddr*) &(sf.dest), sizeof(struct sockaddr));
		//printf("Sent length: %d\n\n\n", sent_length);
		if (sent_length != msg_len) printf("Message not sent entirely\n\n");
		else printf("Message sent\n\n");
		
		pthread_mutex_unlock(&writeMutex);
	
	}

}


void *receive_message(void *sock_ptr) {

	int sockfd = *((int *) sock_ptr);
	
	while (1) {	
	
		// Building addr_in to identify the replying server
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
		pthread_mutex_lock(&writeMutex);
		//printf("Length of the received message: %d\n", recv_len);
		printf("Received message: \n%s\n", buf);
		pthread_mutex_unlock(&writeMutex);
	
	}

}



