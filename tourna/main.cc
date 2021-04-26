#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "peterson_sem.h"

#define N_THR 10

int x = 0;
bool start = false;
PetersonSem p_sem(N_THR);

void* func(void * id)
{
	int* theId = (int*)id;

	while(!start);

	for(int i = 0; i < N_THR; i++)
	{
		p_sem.wait(*theId);
		x++;
		p_sem.signal(*theId);
		printf("Thread %d increments x to %d\n", *theId, x);
	}
	return NULL;
}

int main()
{
	pthread_t tid[N_THR];
	int tid2[N_THR];
	int error;
	for(int i = 0; i < N_THR; i++)
	{
		tid2[i] = i;
		if (error = pthread_create(&tid[i], NULL, func, &tid2[i]))
		{
			printf("Failed to create thread: %s\n", strerror(error));
			exit(-1);
		}	
	}

	start = true;

	for(int i = 0; i < N_THR; i++)
	{
		if (error = pthread_join(tid[i],NULL))
		{
			fprintf(stderr, "Failed to join thread: %s\n", strerror(error));
			exit(-1);
		}	
	}
	printf("Final value of x: %d\n", x);
	return 0;
}

