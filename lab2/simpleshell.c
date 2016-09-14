#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define SIZE 256
#define NUMOFWORDS 25

/*
 * Authors: Andrew Michienzi and Vignesh Suresh
 *
 * The purpose of this code is to learn the forking process and create a simple  * Unix shell 
 */
int main()
{
	struct timeval total_time;
	int previous_sec, previous_usec;
	long previous_ics = 0;
	previous_sec = 0;
	previous_usec=0;
  	//fgets adds a new line to the end of a string
  	char *quit;
	quit = "quit";
	printf("\nMax number of characters:\t%d\n", SIZE);
	printf("Max number of args:\t\t%d\n", NUMOFWORDS);

 	while(1){
  		char *datain;
  		datain = malloc(SIZE);
		//Creating cursor
  		printf("\n");
  		printf(">>");
  		fgets(datain, SIZE, stdin);
		//if datain is empty
		if(!strcmp("\n", datain)){
			free(datain);
			continue;
		}
		//Remove new line at end from fgets
		datain = strtok(datain, "\n");
  		//Quit if correct input
 		if(!strcmp(quit, datain)){
    			free(datain);
    			exit(0);
  		}
	  	char* command[NUMOFWORDS];	
  		int i = 0;
  		command[i] = strtok(datain, " ");
		do{
			i++;
			command[i] = strtok(NULL, " ");
		}while(command[i]);
  		pid_t pid;
  		int status;
		//Fork and Failure catch
  		if((pid=fork())<0){
    			perror("fork failure");
    			free(datain);
			free(quit);
    			exit(1);
  		}
    		//Child. Execute and catch failure
  		if(pid==0){
    			execvp(command[0], &command[0]);
      			perror("\nexec failedyooooooo");
      			free(datain);
      			exit(1);	
  		}
    		//Parent
  		else{
    			wait(&status);
			
			//Find CPU usage time
			struct rusage usage;
			int sec, usec;
			long ics;
			getrusage(RUSAGE_CHILDREN, &usage);
			
			//Find time
			total_time = usage.ru_utime;
			sec = total_time.tv_sec - previous_sec;
			previous_sec = sec;
			usec = total_time.tv_usec - previous_usec;
			previous_usec = usec;
			printf("\nUsage time:\t%ds, %dus", sec, usec);
 			
			//Find involuntary context switches
			ics = usage.ru_nivcsw - previous_ics;
			printf("\nICS:\t\t%ld\n", ics);
			//printf("usage.ru_nivcsw:\t%ld", usage.ru_nivcsw);
			//printf("\nprevious_ics:\t%ld\n", previous_ics);
			previous_ics = ics;
		}	
		free(datain);
	}
	free(quit);
  	return (0);
}

