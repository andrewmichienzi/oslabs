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

/*
* This program is used to simulate creating multiple threads to write to different files
*/

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
	//Need to detach so the thread will end on its own
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
		//20% chance we sleep for 7-10 seconds to simulate not finding file
		sleep((r%3)+7);
		printf("Could not find %s", file);
		fflush(stdout);
	}
	else{
		//80% Simulates writing to file
		sleep(1);
		printf("Printed to %s", file);
		requests_S++;
		fflush(stdout);
	}
	pthread_exit(NULL);
}

void sigHandler(int sigNum){
	//Prints off information about threads
	printf("\n\nRequests received:\t%d\n", requests_R);
	printf("Requests serviced:\t%d\n", requests_S);
	fflush(stdout);
	exit(0);	
}

/* Sample Output

Filename:	Andrew
Filename:	Is
Filename:	Printed to Andrewcooler
Filename:	than
Filename:	Printed to coolerneshPrinted to than
Filename:	Printed to neshforever
Filename:	Printed to foreverCould not find Is^C

Requests received:	6
Requests serviced:	5

*/
