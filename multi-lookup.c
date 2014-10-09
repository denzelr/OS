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

int main(int argc, char* argv[]){

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