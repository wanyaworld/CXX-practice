#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NITER 30
#define NTHR 30

sem_t sem_ticket, sem_barrier0, sem_barrier1;
int counter0 = 0, counter1 = 0;

void *run(void *args) {
	int id = *((int *)args);
	int i;
	for (i = 0; i < NITER; i++) {
		sem_wait(&sem_ticket);
		counter0++;
		sem_post(&sem_ticket);

		if (counter0 == NTHR) {
			sem_trywait(&sem_barrier1);
			counter0 = 0;
			sem_post(&sem_barrier0);
		}

		sem_wait(&sem_barrier0);
		sem_post(&sem_barrier0);
		
		printf("Thread %d in iteration %d\n", id, i);
		
		sem_wait(&sem_ticket);
		counter1++;
		sem_post(&sem_ticket);
		
		if (counter1 == NTHR) {
			sem_trywait(&sem_barrier0);
			counter1 = 0;
			sem_post(&sem_barrier1);
		}

		sem_wait(&sem_barrier1);
		sem_post(&sem_barrier1);
	}
}

int main()
{
	pthread_t tid[NTHR];
	int id[NTHR];
	int i;
	sem_init(&sem_ticket, 0, 1);
	sem_init(&sem_barrier0, 0, 0);
	sem_init(&sem_barrier1, 0, 0);
	for (i = 0; i < NTHR; i++)
	{
		id[i]=i+1;
		pthread_create(&tid[i], NULL, run, (void *)&id[i]);
	}
	pthread_exit(NULL);
	
	sem_destroy(&sem_ticket);
	sem_destroy(&sem_barrier0);
	sem_destroy(&sem_barrier1);
	
	return 0;
}

