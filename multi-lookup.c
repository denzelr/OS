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
#define THREADMAX 4

pthread_mutex_t m;
pthread_cond_t prod;
pthread_cond_t con;

int main(int argc, char* argv[]){
	//argument error handeling
	//queue_init(queue *q, QUEUEMAXSIZE); && check for queue failure
	
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
	//In each file, iterate through lines
	//Create mutex lock
	//wait if queue is full
	//queue push address
	//send signal
	//unlock mutex
	//close input file
	

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
	//while were not finished or there is stuff in queue
	//		take out mutex lock
	//		condition handeling
	//		pop a host name off queue
	//      DNS lookup
	//		print to output file
	//		signal
	//		unlock
	//		exit thread??

	while(!finished || counter > 0){
		pthread_mutex_lock(&m);
		while(count == 0){
			pthread_cond_wait(&con, &m);
		}
		consume();
		pthread_cond_signal(&prod, &m);
	}

}
