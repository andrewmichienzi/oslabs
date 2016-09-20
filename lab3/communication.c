#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void USR1Handler(int);

int main()
{ 

    	// use these variables
	
	//Set up sigaction
	struct sigaction usr1_action;
	sigset_t block_mask;
	
	sigfillset(&block_mask);
	usr1_action.sa_handler = USR1Handler;
	usr1_action.sa_mask = block_mask;
	usr1_action.sa_flags = 0;
	sigaction(SIGUSR1, &usr_action, NULL);

    	if ((pid = fork()) < 0) {
        	perror("fork failure");
        	exit(1);
    	}
    	else if (pid == 0) {
        	//child
        	int i;
		srand((unsigned) time(&t));

        	while(1){
			i = rand() % 7;
			if(i%2){
				kill(getppid(), SIGUSR1);
			
			}
			else{
				//SIGUSR2 Signal
				printf("SIGUSR2\n");
			}	
		}
		exit(0);
    	}
    	else {
    		//parent
    		while(1);
    	}
    	return 0;
}

void USR1Handler(int sigNum){
	printf("SIGUSR1\n");
}
