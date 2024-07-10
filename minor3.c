/* minor3.c - using producer-consumer paradigm. */
/**
 * Emilio Pena
 * 3600.003
 * 10/19/2023
 * 
 * empty&full buffers have been added
 * to help with reducing the CPU percentage
 * by calling wait and signal thread functions 
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NITEMS 10		// number of items in shared buffer

// shared variables
char shared_buffer[NITEMS];	// echo buffer
int shared_count;		// item count

//conditional variables
pthread_cond_t fullBuffer = PTHREAD_COND_INITIALIZER; //shared buffer is full
pthread_cond_t emptyBuffer = PTHREAD_COND_INITIALIZER; //shared buffer is empty

pthread_mutex_t mutex;		// pthread mutex
unsigned int prod_index = 0; 	// producer index into shared buffer
unsigned int cons_index = 0; 	// consumer index into shard buffer

// function prototypes
void * producer(void *arg);
void * consumer(void *arg);

int main() 
{ 
	pthread_t prod_tid, cons_tid1, cons_tid2; 

	// initialize pthread variables
	pthread_mutex_init(&mutex, NULL);
	//my commands
	pthread_cond_init(&fullBuffer, NULL);
	pthread_cond_init(&emptyBuffer, NULL);
	
	// start producer thread
	pthread_create(&prod_tid, NULL, producer, NULL);

	//

	// start consumer threads
	pthread_create(&cons_tid1, NULL, consumer, NULL);
	pthread_create(&cons_tid2, NULL, consumer, NULL);
	
	// wait for threads to finish
	pthread_join(prod_tid, NULL);
	pthread_join(cons_tid1, NULL);
	pthread_join(cons_tid2, NULL);
			
	// clean up
	pthread_mutex_destroy(&mutex);
	//my commands
	pthread_cond_destroy(&emptyBuffer);
	pthread_cond_destroy(&fullBuffer);
	
	return 0;
}

// producer thread executes this function
void * producer(void *arg)
{
	char key;

	printf("Enter text for producer to read and consumer to print, use Ctrl-C to exit.\n\n");

	// this loop has the producer read data in from stdin and place the read data on the shared buffer
	while (1)
	{
		// read input key
		scanf("%c", &key);
		pthread_mutex_lock(&mutex);
		// this loop is used to poll the shared buffer to see if it is full:
		// -- if full, unlock and loop again to keep polling
		// -- if not full, keep locked and proceed to place character on shared buffer

			// if buffer is full, release mutex lock and check again
			while(shared_count == NITEMS)
				{
				pthread_cond_wait(&emptyBuffer,&mutex);//critical section to wait until buffer is empty
				}

		// store key in shared buffer
		shared_buffer[prod_index] = key;

		// update shared count variable
		shared_count++;

		// update producer index
		if (prod_index == NITEMS - 1)
			prod_index = 0;
		else
			prod_index++;
		
		pthread_cond_signal(&fullBuffer);
		// release mutex lock
		pthread_mutex_unlock(&mutex); 
	}

	return NULL;
}

// consumer thread executes this function
void * consumer(void *arg)
{
	char key;

	long unsigned int id = (long unsigned int)pthread_self();

	// this loop has the consumer get data from the shared buffer and print to stdout
	while (1)
	{
		// this loop is used to poll the shared buffer to see if it is empty:
		// -- if empty, unlock and loop again to keep polling
		// -- if not empty, keep locked and proceed to get character from shared buffer

			// acquire mutex lock
			pthread_mutex_lock(&mutex);

			// if buffer is empty, release mutex lock and check again
			while(shared_count == 0)
			{
				pthread_cond_wait(&fullBuffer, &mutex);
			}
			

		// read key from shared buffer
		key = shared_buffer[cons_index];
		
		// echo key
		printf("consumer %lu: %c\n", (long unsigned int) id, key);

		// update shared count variable
		shared_count--;

		// update consumer index
		if (cons_index == NITEMS - 1)
			cons_index = 0;
		else
			cons_index++;
		pthread_cond_signal(&emptyBuffer);
		// release mutex lock
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}


