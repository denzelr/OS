#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
void* producer(void *arg);
void* consumer(void *arg);
queue q;
int cores;

int main(int argc, char* argv[]){
	//argument error handeling
	//pthread_mutex_init(&m, NULL);
	//pthread_cond_init(&prod, NULL);
	//pthread_cond_init(&con, NULL);
	cores = sysconf(_SC_NPROCESSORS_ONLN);
	int numconthreads = (2*cores);
	printf("we are running %i producer threads\n", (argc - 2));
	

	printf("machine has %i cores\n", cores);
	printf("so we will run %i consumer threads\n", numconthreads);
	if(argc < (MINARGS)) {
		fprintf(stderr, "Error: Does not have the required arguments\n");
		exit(1);
	}

	if((argc-2) > MAX_INPUT_FILES){
		fprintf(stderr, "Error: to many input files max is 10\n");
		exit(-1);
	}

	//  check for queue failure
	if(queue_init(&q, SBUFSIZE) == QUEUE_FAILURE){
		fprintf(stderr, "Error: failure initializing queue\n");
		exit(-1);
	}

	pthread_t prodthreads[MAX_INPUT_FILES];
	//int numprodthreads = 0;
	
	//for(all files) 
		//create producer thread
	int i;
	
	for(i = 1; i < (argc-1); i++){
		FILE* file = fopen(argv[i], "r");
		if (!file) {
			fprintf(stderr, "Error: Could not open file %s\n", argv[i]);
			exit(-1);
		}
		pthread_create(&prodthreads[i], NULL, producer,  file);
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
	for(i=0; i<numconthreads; i++){
		pthread_create(&conthreads[i], NULL, consumer,  outfile);
	}



	//Join producer threads
	for(i=1; i<(argc-2); i++){
		pthread_join(prodthreads[i], NULL);
	}
	done = 1;
	//free(prodthreads);
	//Join consumer threads
	for(i=0; i < numconthreads; i++) {
		pthread_join(conthreads[i], NULL);
	}
	//free(conthreads);
	//close output file
	fclose(outfile);
	//free(outfile);
	queue_cleanup(&q);
	//free(&q);
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

	char name[SBUFSIZE];

	fprintf(stderr, "prod thread spawn\n");
	
	while(fscanf(file, INPUTFS, name) > 0){

		pthread_mutex_lock(&m);
		while(queue_is_full(&q)){
			printf("%s\n", "producer asleep" );
			pthread_cond_wait(&prod, &m);
		}
		char* temp = malloc(SBUFSIZE*sizeof(char));
		strcpy(temp, name);
		queue_push(&q, temp);
		//printf("queue push: %s\n", name);
		pthread_cond_signal(&con);
		pthread_mutex_unlock(&m);
		//free(temp); bad things happen but no more leak
	}
	fclose(file);
	//free(name);
	//free(file);
	//free(arg);
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
	
	printf("con thread spawn\n");
	char* host;
	while(!done || !queue_is_empty(&q)){/*
		
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
	}*/	
		host = queue_pop(&q);
		if(host == NULL){
			printf("consumer sleeping\n");
			pthread_cond_wait(&con, &m);
			//pthread_mutex_unlock(&m);
			//usleep(100);
		}
		else if (dnslookup(host, firstipstr, sizeof(firstipstr)) == UTIL_FAILURE){
			fprintf(stderr, "ERROR: dnslookup error: %s\n", host);
		}
		else{
			fprintf(file, "%s, %s\n", host, firstipstr);
			//printf ("resolved: %s, %s\n", host, firstipstr);
			//free(host);
			
		}
		free(host);
		pthread_cond_signal(&prod);
		pthread_mutex_unlock(&m);
	}
	//free(file);
	//free(arg);	
	pthread_exit(file);

}