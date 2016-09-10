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

  		printf("\n\n");
  		printf(">>");
  		fgets(datain, SIZE, stdin);
		if(!strcmp("\n", datain)){
			printf("test");
			free(datain);
			continue;
		}
		datain = strtok(datain, "\n");
  		//Quit if correct input
 		if(!strcmp(quit, datain)){
    			free(datain);
    			exit(0);
  		}
  		/*for(i = 0; i < ; i++){
    			command[i] = strsep(&datain, " ");
  		}*/
	  	char* command[50];	
  		int i = 0;
  		command[i] = strtok(datain, " ");
		printf("command[%d] = %s", i, command[i]);
		do{
			i++;
			command[i] = strtok(NULL, " ");
			printf("\ncommand[%d] = %s", i, command[i]);
		}while(command[i]);
		command[i] = '\0';	
  		pid_t pid, child;
  		int status;

  		if((pid=fork())<0){
    			perror("fork failure");
    			free(datain);
    			exit(1);
  		}
		printf("\npid = %d", (int)pid);		

  		if(pid==0){
    			//child
    			printf("\nChild\n");
    			execvp(command[0], &command[0]);
      			perror("\nexec failedyooooooo");
      			free(datain);
      			exit(1);	
  		}
  		else{
    			//Parent
    			printf("\nParent\n");
    			child = wait(&status);
			printf("\nchild = %d", (int) child);
 		}
  		printf("command 0 = %s",command[0]);
  		printf("\n");
  		printf("command 1 = %s",command[1]);
  		printf("command 2 = %s", command[2]);
		free(datain);
	}
	free(quit);
  	return (0);
}

