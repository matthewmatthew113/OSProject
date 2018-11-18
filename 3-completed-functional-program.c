#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "buffer.h"



buffer_item buffer[BUFFER_SIZE];
int buffer_index;
 
sem_t mutex; 
sem_t full;  
sem_t empty; 


int insert_item(buffer_item value) {
    
	sem_wait(&mutex);
	if (buffer_index < BUFFER_SIZE) {
        buffer[buffer_index++] = value;
    } 
	else {
		sem_post(&mutex);
		return -1;
    }
	sem_post(&mutex);
	return 0;
}
 
int remove_item(buffer_item *item) {
    sem_wait(&mutex);
	if (buffer_index > 0) {
        *item = buffer[--buffer_index];
    } 
	else {
		sem_post(&mutex);
		return -1;
    }
	sem_post(&mutex);
	return 0;
}
 
 
void *producer(void *param) {
    buffer_item item;
    while (1) {
        sleep( rand() % 4);
        item = rand();
        sem_wait(&full); 
        if(insert_item(item))
			printf("report error condition");
		else
			printf("Producer produced %d\n", item);
        sem_post(&empty);
    }
    pthread_exit(0);
}
 
void *consumer(void *param) {
    buffer_item item;
    
    while (1) {
		sleep( rand() % 4);
        sem_wait(&empty);

        if(remove_item(&item))
			printf("report error condition");
		else
			printf("Consumer consumed %d\n", item);
        sem_post(&full); 
   }
    pthread_exit(0);
}
 
int main(int argc, char *argv[]) {
    
	int numSleep = atoi(argv[1]);
	int numProd = atoi(argv[2]);
	int numCon = atoi(argv[3]);
	buffer_index = 0;
	
    
	int check = sem_init(&mutex, 0, 10);
    int check2 = sem_init(&full,0, BUFFER_SIZE); 
    int check3 = sem_init(&empty,0,0);
	
	if(check1 == 0 && check2 == 0 && check3 == 0){
	
		pthread_t prodThread[numProd];
		pthread_t conThread[numCon];
		for (int i = 0; i < numProd; i++)
			pthread_create(&prodThread[i], NULL, producer, NULL);
		for (int i = 0; i < numCon; i++)
			pthread_create(&conThread[i], NULL, consumer, NULL);

	 
		for (int i = 0; i < numProd; i++)
			pthread_join(prodThread[i], NULL);
		for (int i = 0; i < numCon; i++)
			pthread_join(conThread[i], NULL);
 
		sleep(numSleep);
		sem_destroy(&mutex);
		sem_destroy(&full);
		sem_destroy(&empty);
	
	}
	else{
		printf("Semaphores not created properly");
	}

   
 
    return 0;
}