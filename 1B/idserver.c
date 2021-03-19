/*
 * idserver.c
 *
 *  Created on: Feb 15, 2016
 *      Author: jiaziyi
 *  Updated: JACF, 2020
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "idserver.h"

/**
 * print the server's information
 */
void print_idserver(idserver s)
{
	printf("Id: %s\n", s.id);
	printf("Latency (usec): %d\n", s.latency);
	printf("Region: %s\n", s.region);
	printf("Status: %s\n", s.status);
	printf("Nbr of threads: %d\n", *s.nthreads);
}

/**
 * try to modify the server information
 */

// Doesn't work because the idserver s in the function is probably a local variable that is different from the original s (so we don't really modify s)
void modify(idserver s, char *id, int latency, char status[])
{

s.id = id;
s.latency = latency;
strncpy(s.status, status, strlen(status));

}

/**
 * try to modify the student information using pointer
 */
void modify_by_pointer(idserver *s, char *id, int latency, char *status)
{

s->id = id;
s->latency = latency;
strncpy(s->status, status, strlen(status));

}

idserver* create_idserver(char *id, char *region, int latency, char *status, int *nthreads)
{
	idserver *s = malloc(sizeof(idserver));
	s->id = malloc(10*sizeof(char));
	s->id = id;
	s->region = malloc(10*sizeof(char));
	s->region = region;
	s->latency = latency;
	
	strncpy(s->status, malloc(10*sizeof(char)), 10*sizeof(char));
	strncpy(s->status, status, strlen(status));
	
	//s->status = malloc(10*sizeof(char));
	//s->status = status;
	s->nthreads = malloc(sizeof(int));
	s->nthreads = nthreads;
	puts("---print inside create_idserver function---");
	print_idserver(*s);
	puts("---end of print inside");
	return s;
}
