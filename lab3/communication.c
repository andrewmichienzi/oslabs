#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int sigNum);


int main()
{ 
	pid_t pid;
    	if ((pid = fork()) < 0) {
        	perror("fork failure");
        	exit(1);
    	}
    	else if (pid == 0) {
        	//child
        	int i;
		long ppid =(long) getppid();
        	while(1){
			i = rand() % 7;
			sleep(i);
			if(i%2){
				kill(ppid, SIGUSR1);	
			}
			else{
				kill(ppid, SIGUSR2);
			}	
		}
		exit(0);
    	}
    	else {
		printf("spawned child PID# %d\n", pid);
    		//parent
    		signal(SIGUSR1, signalHandler);
		signal(SIGUSR2, signalHandler);
		signal(SIGINT, signalHandler);
    		while(1){
			printf("waiting...\t");
			pause();
		}
    	}
    	return 0;
}

void signalHandler(int sigNum){
	if(sigNum == SIGUSR1)
		printf("received a SIGUSR1 signal\n");
	if(sigNum == SIGUSR2)
		printf("received a SIGUSR2 signal\n");
	if(sigNum == SIGINT){
		printf("received. That's it, I'm shutting you down\n");
		exit(0);
	}
}
