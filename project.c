//Marc Anthony Franze, Matthew Souter and Dyllon Ngmon

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "buffer.h"


//Created global variables
buffer_item buffer[BUFFER_SIZE];
int buffer_index;
 
sem_t mutex; 
sem_t full;  
sem_t empty; 

//Buffer insert function
int insert_item(buffer_item value) {
    
	sem_wait(&mutex);
	
	//Checks if the buffer has room to insert in
	if (buffer_index < BUFFER_SIZE) {
        buffer[buffer_index++] = value;
    } 
	else {
		//Returns -1 on failure
		sem_post(&mutex);
		return -1;
    }
	sem_post(&mutex);
	return 0;
}

//Buffer remove function
int remove_item(buffer_item *item) {
    
	sem_wait(&mutex);
	
	//Checks if the buffer can be removed from
	if (buffer_index > 0) {
        *item = buffer[--buffer_index];
    } 
	else {
        //Returns -1 on failure
		sem_post(&mutex);
		return -1;
    }
	sem_post(&mutex);
	return 0;
}
 
//Producer function
void *producer(void *param) {
    buffer_item item;
    while (1) {
        //Sleeps for random time
		sleep( rand() % 4);
        
		//Creates a random number and adds it to the buffer
		item = rand();
        sem_wait(&full); 
        if(insert_item(item))
			fprintf(stderr, "Error: %m\n");
		else
			printf("Producer produced %d\n", item);
        sem_post(&empty);
    }
    pthread_exit(0);
}

//Consumer function
void *consumer(void *param) {
    buffer_item item;
    
    while (1) {
		//Sleeps for random time
		sleep( rand() % 4);
        
		//Removes a number from the buffer
		sem_wait(&empty);
        if(remove_item(&item))
			fprintf(stderr, "Error: %m\n");
		else
			printf("Consumer consumed %d\n", item);
        sem_post(&full); 
   }
    pthread_exit(0);
}
 
int main(int argc, char *argv[]) {
    
	//Checks if the user has entered the correct number of arguments
	if( argc != 4){
		printf("Usage: ./project <sleep number> <number of producers> <number of consumers>\n"); 
		return 0;
	}
	
	//Arguments
	int numSleep = atoi(argv[1]);
	int numProd = atoi(argv[2]);
	int numCon = atoi(argv[3]);
	
	//Initializes buffer
	buffer_index = 0;
	
	//Checks if the arguments are entered correctly
	if(numSleep <= 0 || numProd <= 0 || numCon <= 0){
		printf("Usage: ./project <sleep number> <number of producers> <number of consumers>\n"); 
		return 0;
	}
    
	//Creates semaphores
	int check1 = sem_init(&mutex, 0, 10);
    int check2 = sem_init(&full,0, BUFFER_SIZE); 
    int check3 = sem_init(&empty,0,0);
	
	//Runs if all the semaphores were created properly
	if(check1 == 0 && check2 == 0 && check3 == 0){
		
		
		//Creates producer and consumer threads
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
	
	}
	else{
		printf("Semaphores not created properly");
	}

   
 
    return 0;
}