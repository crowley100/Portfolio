#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum { FALSE, TRUE } boolean;

#define checkResults(string, val) {             \
if (val) {                                     \
printf("Failed with %d at %s", val, string); \
exit(1);                                     \
}                                              \
}

#define                 NUMTHREADS     5
pthread_mutex_t         dataMutex = PTHREAD_MUTEX_INITIALIZER;			
pthread_cond_t          dataPresentCondition = PTHREAD_COND_INITIALIZER;	
pthread_cond_t          dataVacancyCondition = PTHREAD_COND_INITIALIZER;
pthread_barrier_t 	hold;

int			buffer[10];
int			items=0;  // to keep track of number of items in buffer

void random_delay()
{
	int r = rand() % 3;
	sleep(r); 
}

void *consumerThread(void *threadid)
{
	int   rc;
	int dataItem;
	printf("Consumer Thread %.8x: Entered\n",(int)threadid);
	pthread_barrier_wait(&hold);
	while (TRUE) {
		random_delay(); // pseudo-random delay
		rc = pthread_mutex_lock(&dataMutex); // ????
		checkResults("pthread_mutex_lock()\n", rc);
		while (!items) { 
			printf("Consumer Thread %.8x: Wait for data to be produced\n",(int)threadid);
			rc = pthread_cond_wait(&dataPresentCondition, &dataMutex);
			if (rc) {
				printf("Consumer Thread %.8x: condwait failed, rc=%d\n",(int)threadid,rc);
				pthread_mutex_unlock(&dataMutex);
				exit(1);
			}
		}
		dataItem = buffer[items-1];
		--items;
		printf("consumer %d: consumed data item %d\n", (int)threadid, dataItem);
		rc = pthread_cond_signal(&dataVacancyCondition); /* wake up a producer  */
		rc = pthread_mutex_unlock(&dataMutex);
	}
	printf("Consumer Thread %.8x: All done\n",(int)threadid);
	rc = pthread_mutex_unlock(&dataMutex);
	checkResults("pthread_mutex_unlock()\n", rc);
	return NULL;
}

void *producerThread(void *threadid)
{	
	int rc;
	// NOTE: COULD CLOSE CONSUMER THREADS IN HERE...
	int amountOfData = 50;
	while(amountOfData--)
	{
//		random_delay(); // pseudo-random delay
		rc = pthread_mutex_lock(&dataMutex);   /* Protect shared data and flag  */
		checkResults("pthread_mutex_lock()\n", rc);
		while (items>=10) { 
			printf("NOTICE: BUFFER FULL\n");
			printf("Producer thread Thread %d: Wait for data to be consumed\n",(int)threadid);
			rc = pthread_cond_wait(&dataVacancyCondition, &dataMutex);
			if (rc) {
				printf("Producer Thread %d: condwait failed, rc=%d\n",(int)threadid,rc);
				pthread_mutex_unlock(&dataMutex);
				exit(1);
			}
		}
		printf("producing data...\n");
		++items;                          /* Add data                      */
		buffer[items-1] = amountOfData;
		rc = pthread_cond_signal(&dataPresentCondition); /* wake up a consumer  */
		if (rc) {
			pthread_mutex_unlock(&dataMutex);
			printf("Producer: Failed to wake up consumer, rc=%d\n", rc);
			exit(1);
		}
		rc = pthread_mutex_unlock(&dataMutex);
		checkResults("pthread_mutex_lock()\n",rc);	
	}
	printf("producer done...\n");
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t             thread[NUMTHREADS];
	int                   rc=0;
	int                   i;
	pthread_barrier_init(&hold, NULL, NUMTHREADS); // barrier so all consumers start at same time

	printf("Enter Testcase - %s\n", argv[0]);
	
	printf("Create/start threads\n");
	for (i=0; i <NUMTHREADS; ++i) {
		rc = pthread_create(&thread[i], NULL, consumerThread, (void *)i);
		checkResults("pthread_create()\n", rc);
	}
	rc = pthread_create(&thread[i], NULL, producerThread, (void *)i);
	
	printf("Wait for the threads to complete, and release their resources\n");
	for (i=0; i <NUMTHREADS; ++i) {
		rc = pthread_join(thread[i], NULL);
		checkResults("pthread_join()\n", rc);
	}
	rc = pthread_join(thread[i], NULL);
	
	printf("Clean up\n");
	rc = pthread_mutex_destroy(&dataMutex);
	rc = pthread_cond_destroy(&dataPresentCondition);
	rc = pthread_barrier_destroy(&hold);
	printf("Main completed\n");
	return 0;
}
