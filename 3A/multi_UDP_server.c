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
	int sock;
	struct sockaddr_in *from;
	int buf_len;
	char *buf;
} Recv_info;


pthread_mutex_t writeMutex = PTHREAD_MUTEX_INITIALIZER;
int active_threads = 0;


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
	if (bind_status != 0) printf("Failed to bind socket\n\n");
	
	printf("Server started, waiting for incoming messages\n\n");

	// Main threads receives messages
	while (1) {
	
		// Building addr_in to identify the client
		struct sockaddr_in *from = malloc(sizeof(struct sockaddr_in));
		int *from_len;
		int temp = (int) sizeof(struct sockaddr);
		from_len = &temp;
		
		// Building received message buffer
		int buf_len = 1000;
		char *buf = malloc(buf_len * sizeof(char));
		memset(buf, 0, 1000);
		
		// Receiving message
		int recv_len = recvfrom(sockfd, buf, buf_len, 0, (struct sockaddr*) from, from_len);
		//printf("Length of the received message: %d\n", recv_len);
		pthread_mutex_lock(&writeMutex);
		printf("Received message: \n%s\n", buf);
		pthread_mutex_unlock(&writeMutex);
		
		// If we have not reached the maximum number of threads, we create a new one to send back the message
		if (active_threads < N_THREAD) {
			
			Recv_info ri;
			ri.sock = sockfd;
			ri.from = from;
			ri.buf_len = recv_len;
			ri.buf = buf;
			
			active_threads++;			
			pthread_t thread;
			if (pthread_create(&thread, NULL, process_message, &ri)) {
				fprintf(stderr, "Failed to create thread\n");
				return 1;
			}
			
		} else {
			pthread_mutex_lock(&writeMutex);
			printf("Message discarded: \n%s\n", buf);
			pthread_mutex_unlock(&writeMutex);
			
			// Sending a message to the client saying we are too busy
			char busy[] = "Sorry, we could not process your message, the server is too busy\n";
			char *busy_buf = busy;
			sendto(sockfd, busy_buf, strlen(busy_buf), 0, (struct sockaddr*) from, sizeof(struct sockaddr));
		}
	
	}
	
}


	
void *process_message(void *ptr) {
	
		
	Recv_info ri = *((Recv_info *) ptr);
	
	// Sleeping because receiving a message is tiring
	usleep(10000000);
		
	// Sending the message back
	int sent_length = sendto(ri.sock, ri.buf, ri.buf_len, 0, (struct sockaddr*) ri.from, sizeof(struct sockaddr));
	if (sent_length != ri.buf_len) {
		pthread_mutex_lock(&writeMutex);
		printf("Message not sent entirely\n\n");
		pthread_mutex_unlock(&writeMutex);
	} else {
		pthread_mutex_lock(&writeMutex);
		printf("Sent back message:\n%s\n", ri.buf);
		pthread_mutex_unlock(&writeMutex);
	}
	//printf("Sent length: %d\n", sent_length);
	
	active_threads--;
	free(ri.buf);
	
	return NULL;

}
