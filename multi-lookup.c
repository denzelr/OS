#include <pthread.h>
#include <stdio.h>
#include <stlib.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"
#include "queue.h"

#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"

pthread_mutex_t m;
pthread_cond_t prod;
pthread_cond_t con;
int threadcount = 4;

queue_init(queue *q, QUEUEMAXSIZE);

int main(int argc, char* argv[]){
	//error handeling
	//for(all files) 
		//create producer thread

	//create consumer thread(arbitrary quantity)
	//for(all consumer threads)
		//call consumer

	//Join producer threads
	//Join consumer threads

	//close output file

}

void producer(){
	while(!finished){
		pthread_mutex_lock(&m);
		while(counter >= SBUFSIZE){
			pthread_cond_wait(&prod, &m);
		}
		produce();
		pthread_cond_signal(&con, &m);
		pthread_mutex_unlock(&m);
}

void consumer(){
	while(!finished || counter > 0){
		pthread_mutex_lock(&m);
		while(count == 0){
			pthread_cond_wait(&con, &m);
		}
		consume();
		pthread_cond_signal(&prod, &m);
	}

}
