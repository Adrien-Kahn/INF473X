/*
 * mutex.c
 *
 *  Created on: Mar 19, 2016
 *      Author: jiaziyi
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 50
void *increase_counter(void *);

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

int counter = 0;

int main() {
	
	for (int k = 0; k < 10; k++) {
		pthread_t thread;
		if (pthread_create(&thread, NULL, increase_counter, NULL)) {
			fprintf(stderr, "Failed to create thread\n");
			return 1;
		}
	
	}

	usleep (1000000);
	printf("\nFinal counter value: %d\n", counter);

}

void *increase_counter(void *arg) {

	pthread_mutex_lock(&mymutex);
	printf("Thread number %ld, working on counter. The current value is %d\n", (long)pthread_self(), counter);
	int i = counter;
	usleep (10000); //simulate the data processing
	counter = i+1;
	pthread_mutex_unlock(&mymutex);
	return NULL;
}

