/*
 * rawip_example.c
 *
 *  Created on: May 4, 2016
 *      Author: jiaziyi
 */


// I took some inspiration from: https://www.binarytides.com/raw-udp-sockets-c-linux/

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include "header.h"

#define SRC_IP  "231.26.45.65" //set your source ip here. It can be a fake one
#define SRC_PORT 8000 //set the source port here. It can be a fake one

#define DEST_IP "21.65.45.123" //set your destination ip here
#define DEST_PORT 6000 //set the destination port here
#define TEST_STRING "test data" //a test string as packet payload

int main(int argc, char *argv[])
{
	char source_ip[] = SRC_IP;
	char dest_ip[] = DEST_IP;


	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    int hincl = 1;                  /* 1 = on, 0 = off */
    setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl));

	if(fd < 0)
	{
		perror("Error creating raw socket ");
		exit(1);
	}

	char packet[65536], *data;
	char data_string[] = TEST_STRING;
	memset(packet, 0, 65536);

	//IP header pointer
	struct iphdr *iph = (struct iphdr *)packet;

	//UDP header pointer
	struct udphdr *udph = (struct udphdr *)(packet + sizeof(struct iphdr));
	struct pseudo_udp_header psh; //pseudo header

	//data section pointer
	data = packet + sizeof(struct iphdr) + sizeof(struct udphdr);

	//fill the data section
	strncpy(data, data_string, strlen(data_string));


	//fill the pseudo header
	if (inet_pton(AF_INET, source_ip, &(psh.source_address)) != 1) {
		printf("Failed to convert source ip address for pseudo header\n\n");
	}
	if (inet_pton(AF_INET, dest_ip, &(psh.dest_address)) != 1) {
		printf("Failed to convert destination ip address for pseudo header\n\n");
	}
	psh.placeholder = 0;
	psh.protocol = 17;
	psh.udp_length = htons(sizeof(struct udphdr) + strlen(data));
	
	printf("Pseudo header filled\n");
	
	
	
	
	//fill the UDP header
	udph->source = htons(SRC_PORT);
	udph->dest = htons(DEST_PORT);
	udph->len = htons(sizeof(struct udphdr) + strlen(data));
	udph->check = 0;
	
	printf("UDP header filled\n");	
	
	
	//UDP checksum

	int psize = sizeof(struct pseudo_udp_header) + sizeof(struct udphdr) + strlen(data);
	char *pseudogram = malloc(psize);
	memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_udp_header));
	memcpy(pseudogram + sizeof(struct pseudo_udp_header) , udph , sizeof(struct udphdr) + strlen(data));
	udph->check = checksum((unsigned short*) pseudogram, psize);

	
	
	
	//fill the IP header here
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
	iph->id = 0;
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = 17;
	iph->saddr = inet_addr(source_ip);
	if (inet_pton(AF_INET, dest_ip, &(iph->daddr)) != 1) {
		printf("Failed to convert destination ip address\n\n");
	}
	//The IP checksum
	iph->check = 0;
	iph->check = checksum((unsigned short *) iph, sizeof(struct iphdr));
	
	printf("IP header filled\n\n");
	
	
	

	
	
	
	
	// Building destination addr_in
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_port = htons(DEST_PORT);
	if (inet_pton(AF_INET, dest_ip, &(dest.sin_addr)) != 1) printf("inet_pton error\n\n");


	printf("iph->tot_len: %d\n", iph->tot_len);


	//send the packet
	int sent_len = sendto(fd, packet, iph->tot_len, 0, (struct sockaddr *) &dest, sizeof(struct sockaddr));
	printf("Sent length: %d\n\n", sent_len);

	return 0;

}
