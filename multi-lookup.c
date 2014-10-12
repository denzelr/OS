#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"
#include "queue.h"

#define MINARGS 3
#define MAX_INPUT_FILES 10
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"
#define THREADMAX 4

pthread_mutex_t m;
pthread_cond_t prod;
pthread_cond_t con;
int done = 0;

int main(int argc, char* argv[]){
	//argument error handeling
	if(argc < MINARGS) {
		fprintf(stderror, "Error: Does not have the required arguments");
		exit(1);
	}

	//  check for queue failure
	if(queue_init(queue *queue, SBUFSIZE) == QUEUE_FAILURE){
		fprintf(stderror, "Error: failure initializing queue");
		exit(-1);
	}

	pthread_t prod[MAX_INPUT_FILES];
	int prodthreads = 0;
	
	//for(all files) 
		//create producer thread
	for(int i = 1; i < (argc-2); i++){
		FILE* file = fopen(argv[i], "r");
		if (!file) {fprintf(stderror, "Error: Could not open file");}
		pthread_create(&prod[i], NULL, producer, file);
		prodthreads++;
	}
	FILE* outfile = fopen(argv[(argc-1)], "w");
	if(!outfile) {
		fprintf(stderror, "ERROR: bad output file");
		exit(0);
	}

	pthread_t con[THREADMAX];

	//create consumer thread(arbitrary quantity)
	//for(all consumer threads)
		//call consumer
	for(i=0; i<THREADMAX; i++){
		pthread_create(&con[i], NULL, consumers, outfile);
	}

	//Join producer threads
	for(i=1; i<(argc-2); i++){
		pthread_join(prod[i]);
	}
	done = 1;
	
	//Join consumer threads
	for(i=1; i < THREADMAX; i++) {
		pthread_join(con[i]);
	}

	//close output file
	fclose(outfile);
	return 1;
}

void producer(FILE *file){
	//In each file, iterate through lines
	//Create mutex lock
	//wait if queue is full
	//queue push address
	//send signal
	//unlock mutex
	//close input file
	char buf[SBUFSIZE];
	
	while(fscanf(file, INPUTFS, buf)){
		pthread_mutex_lock(&m);
		while(queue_is_full(&queue)){
			pthread_cond_wait(&prod, &m);
		}
		queue_push(&queue, buf);
		pthread_cond_signal(&con, &m);
		pthread_mutex_unlock(&m);
	}
	fclose(file);
	pthread_exit(file);
}

void consumer(FILE *file){
	//while were not finished or there is stuff in queue
	//		take out mutex lock
	//		condition handeling
	//		pop a host name off queue
	//      DNS lookup
	//		print to output file
	//		signal
	//		unlock
	//		exit thread??
	char firstipstr[INET6_ADDRSTRLEN]

	while(!done || !queue_is_empty()){
		pthread_mutex_lock(&m);
		while(count == 0){
			pthread_cond_wait(&con, &m);
		}
		void* temp = queue_pop(&queue);
		if (dnslookup(temp, firstipstr, size(firstipstr)) == UTIL_FAILURE){
			fprintf(stderror, "ERROR: dnslookup error: %s\n", buf);
		}
		fprintf(outfile, "%s,%s\n", buf, firstipstr);
		pthread_cond_signal(&prod, &m);
		pthread_mutex_unlock(&m);
	}
	pthread_exit(outfile);

}
