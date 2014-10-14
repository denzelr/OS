#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "util.h"
#include "queue.h"

#define MINARGS 5
#define MAX_INPUT_FILES 10
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"
#define THREADMAX 4

pthread_mutex_t m;
pthread_cond_t prod;
pthread_cond_t con;
int done = 0;
void* producer(void *arg);
void* consumer(void *arg);
queue q;

int main(int argc, char* argv[]){
	//argument error handeling

	if(argc < (MINARGS)) {
		fprintf(stderr, "Error: Does not have the required arguments\n");
		exit(1);
	}

	//  check for queue failure
	if(queue_init(&q, SBUFSIZE) == QUEUE_FAILURE){
		fprintf(stderr, "Error: failure initializing queue\n");
		exit(-1);
	}

	pthread_t prodthreads[MAX_INPUT_FILES];
	int numprodthreads = 0;
	
	//for(all files) 
		//create producer thread
	int i;
	
	for(i = 1; i < (argc-1); i++){
		FILE* file = fopen(argv[i], "r");
		if (!file) {fprintf(stderr, "Error: Could not open file\n");}
		pthread_create(&prodthreads[i], NULL, producer,  file);
		numprodthreads++;
	}
	
	FILE* outfile = fopen(argv[(argc-1)], "w");
	if(!outfile) {
		fprintf(stderr, "ERROR: bad output file\n");
		exit(0);
	}

	pthread_t conthreads[THREADMAX];

	//create consumer thread(arbitrary quantity)
	//for(all consumer threads)
		//call consumer
	for(i=0; i<THREADMAX; i++){
		pthread_create(&conthreads[i], NULL, consumer,  outfile);
	}



	//Join producer threads
	for(i=1; i<(argc-2); i++){
		pthread_join(prodthreads[i], NULL);
	}
	done = 1;
	
	//Join consumer threads
	for(i=1; i < THREADMAX; i++) {
		pthread_join(conthreads[i], NULL);
	}

	//close output file
	fclose(outfile);
	return 1;
}

void* producer(void *arg){
	//In each file, iterate through lines
	//Create mutex lock
	//wait if queue is full
	//queue push address
	//send signal
	//unlock mutex
	//close input file
	FILE* file = arg;
	pthread_mutex_lock(&m);
	char name[SBUFSIZE];
	fprintf(stderr, "prod thread spawn\n");
	
	while(fscanf(file, INPUTFS, name) > 0){

		
		while(queue_is_full(&q)){
			printf("%s\n", "producer asleep" );
			pthread_cond_wait(&prod, &m);
		}
		queue_push(&q, name);
		printf("queue push: %s\n", name);
		pthread_cond_signal(&con);
		pthread_mutex_unlock(&m);
	}
	fclose(file);
	pthread_exit(file);
}

void* consumer(void* arg){
	//while were not finished or there is stuff in queue
	//		take out mutex lock
	//		condition handeling
	//		pop a host name off queue
	//      DNS lookup
	//		print to output file
	//		signal
	//		unlock
	//		exit thread??
	FILE* file = arg;
	char firstipstr[INET6_ADDRSTRLEN];
	pthread_mutex_lock(&m);
	char* host = malloc(SBUFSIZE * sizeof(char));
	fprintf(stderr, "con thread spawn\n");

	while(!done || !queue_is_empty(&q)){
		
		while(queue_is_empty(&q)){
			printf("%s\n","consumer sleeping" );
			pthread_cond_wait(&con, &m);
		}
		host = queue_pop(&q);
		//printf("host is: %s\n", host);
			if (dnslookup(host, firstipstr, sizeof(firstipstr)) == UTIL_FAILURE){
				fprintf(stderr, "ERROR: dnslookup error: %s\n", host);
			}
			fprintf(file, "%s, %s\n", host, firstipstr);
			printf ("resolved: %s, %s\n", host, firstipstr);
			pthread_cond_signal(&prod);
			pthread_mutex_unlock(&m);
	}
	pthread_exit(file);

}