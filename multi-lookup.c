#include <pthread.h>
#include <stdio.h>
#include <stlib.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"

#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"

pthread_mutex_t m;
pthread_cond_t prod;
pthread_cond_t con;
int threadcount = 4;

int main(int argc, char* argv[]){
	file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("Error: Opening File");
		exit(1);
	}
	int lines = 1;
	int ch = 0;
	while(!feof(file)){
		ch = fgets(file)
		if (ch == "\n"){
			lines++;
		}
	}
	int i;
	for(i=0;i=lines;i++){
		read = fgets(i, size(i), "%s");
		//pthread__t producers();
		printf("\s", read);
	}
	exit(0)
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
		pthrad_mutex_lock(&m);
		while(count == 0){
			pthread_cond_wait(&con, &m);
		}
		consume();
		pthread_cond_signal(&prod, &m);
	}

}

void produce(){

}

void consume(){

}