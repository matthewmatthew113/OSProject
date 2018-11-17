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
    
}
 
int remove_item(buffer_item *item) {

}
 
 
void *producer(void *param) {

}
 
void *consumer(void *param) {

}
 
int main(int argc, char *argv[]) {
    

}