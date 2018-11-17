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
	
	sem_post(&mutex);
	return 0;
}
 
int remove_item(buffer_item *item) {
    sem_wait(&mutex);
	
	sem_post(&mutex);
	return 0;
}
 
 
void *producer(void *param) {
    buffer_item item;
    while (1) {
        
    }
    pthread_exit(0);
}
 
void *consumer(void *param) {
    buffer_item item;
    
    while (1) {
		
   }
    pthread_exit(0);
}
 
int main(int argc, char *argv[]) {
    
	int numSleep = atoi(argv[1]);
	int numProd = atoi(argv[2]);
	int numCon = atoi(argv[3]);
	buffer_index = 0;
	
    
	sem_init(&mutex, 0, 10);
    sem_init(&full,0, BUFFER_SIZE); 
    sem_init(&empty,0,0);

    pthread_t prodThread[numProd];
	pthread_t conThread[numCon];

 
	sleep(numSleep);
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
 
    return 0;
}