#include <pthread.h> 
#include <stdio.h> 
#include <errno.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include <signal.h>
int requests_R = 0;
int requests_S = 0;
int MAX_THREADS = 4096;
void* writeToFile(void* filename);
void sigHandler(int);
int main() 
{ 
signal (SIGINT, sigHandler); 
char * filename = malloc(sizeof(char)*256);
pthread_t * threads = malloc(sizeof(pthread_t)*MAX_THREADS);
int counter = 0;
int status;
srand(time(NULL));
while(1){
	//Get filename and put it in the global variable
	printf("Filename:\t");
	scanf("%s", filename);
	
	if ((status = pthread_create (&threads[counter], NULL,  writeToFile, filename)) != 0) { 
		fprintf (stderr, "thread create error %d: %s\n", status, strerror(status)); 
	        exit (1); 
	} 
	else{
		if(pthread_detach(*(threads + counter))){
			printf("Error with Detach");
			exit(1);
		}	
	}	
counter++;
}
return 0; 
}

void* writeToFile(void* filename) 
{
	char *file = strdup((char *) filename);
	requests_R++;
	int r = rand();
	if((r % 5) > 3){
		//20%
		sleep((r%3)+7);
		printf("Could not find %s", file);
		fflush(stdout);
	}
	else{
		//80%
		sleep(1);
		printf("Printed to %s", file);
		requests_S++;
		fflush(stdout);
	}
	pthread_exit(NULL);
}

void sigHandler(int sigNum){
	printf("\n\nRequests received:\t%d\n", requests_R);
	printf("Requests serviced:\t%d\n", requests_S);
	fflush(stdout);
	exit(0);	
}
