/**
 * Emilio Pena 10/24/2023
 * 
 * (1) declare the mutex lock, 
 * (2) initialize the mutex lock, 
 * (3) lock and unlock the mutex lock at an appropriate location that results in the code
 *  working as expected, and 
 * (4) destroy the mutex lock. Be sure to place the mutex locks
 *  so that your program works correctly every time.
*/


#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define SIZE 50

char sentence[2000];
int  ind = 0;

//mutex creation and declaration
pthread_mutex_t mutex;

char convertUppercase(char lower)
{
	//Converts lowercase un uppercase
	if ((lower > 96) && (lower < 123))
	{
		return (lower - 32);
	}
	else
	{
		return lower;
	}
}

void printChar()
{
	//prints the converted sentence
	pthread_mutex_lock(&mutex);
	printf("The new sentence is [%d]: \t%c\n", ind, sentence[ind]);
	ind++;
	pthread_mutex_unlock(&mutex);
}

void *convertMessage(void *ptr)
{
	// Function that each threads initiates its execution
	if (ind % 2)
	{
		sentence[ind] = convertUppercase(sentence[ind]);
	}
	
	printChar();

	return 0;
}

int main()
{
	int i;
	char buffer[SIZE];
	char *p;
	pthread_t ts[SIZE]; // define up to 50 threads
	pthread_mutex_init(&mutex, NULL); //initalized mutex

	printf("Please enter a phrase (less than 50 characters): ");

	if (fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
		if ((p = strchr(buffer, '\n')) != NULL)
		{
			pthread_mutex_lock(&mutex);
			*p = '\0';
			pthread_mutex_unlock(&mutex);
		}
	}

	strcpy(sentence, buffer); // copy string to char array
	
	printf("The original sentence is: \t %s\n", sentence);

	// create one thread for each character on the input word
	for (i = 0; i < strlen(buffer) + 1; ++i)
	{
		pthread_create(&ts[i], NULL, convertMessage, NULL);
	}
	
	// we wait until all threads finish execution
	for (i = 0; i < strlen(buffer); i++)
	{
		pthread_join(ts[i], NULL);
	}
	
	printf("\n");
	
	pthread_mutex_destroy(&mutex); //deletes the mutex

	return 0;
}