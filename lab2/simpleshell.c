#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 50
int main()
{
  	char *quit;
  	//fgets adds a new line to the end of a string
	quit = "quit";
 	while(1){ 
  		char *datain;
  		datain = malloc(SIZE);

  		printf("\n");
  		printf(">>");
  		fgets(datain, SIZE, stdin);
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
	  	char* command[50];	
  		int i = 0;
  		command[i] = strtok(datain, " ");
		do{
			i++;
			command[i] = strtok(NULL, " ");
		}while(command[i]);
		//command[i] = '\0';	
  		pid_t pid;
  		int status;

  		if((pid=fork())<0){
    			perror("fork failure");
    			free(datain);
			free(quit);
    			exit(1);
  		}
  		if(pid==0){
    			//child
    			execvp(command[0], &command[0]);
      			perror("\nexec failedyooooooo");
      			free(datain);
      			exit(1);	
  		}
  		else{
    			//Parent
    			wait(&status);
 		}
		free(datain);
	}
	free(quit);
  	return (0);
}

