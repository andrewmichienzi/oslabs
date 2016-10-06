#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>

void signalHandler(int signal);

char *shm, *s;
int shmid;

int main()
{
    char c;
    key_t key;
	char userIn[500];

    //Name for the shared memory segment
    key = 6000;

	//Installs signal handler
	signal(SIGINT, signalHandler);

    //Create the shared memory segment
    if ((shmid = shmget(key, 25, IPC_CREAT)) < 0) {
        perror("Error creating segment");
        exit(1);
    }

    //Attach segment to address space
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("Error attaching segment");
        exit(1);
    }

	//Main memory loop
	while(1){

		//Get user input		
		s = shm;
		fgets(userIn, sizeof(userIn), stdin);

		//Set segment memory to user input
		int i;
		for (i=0; i <= sizeof(userIn); i++)
		    *s++ = userIn[i];

		//Add terminating character at the end of user input
		*s++ = '\0';

		//This checks if both readers have read from memory
		while (*shm != '&')
		    sleep(1);
	}

    return 0;
}

//Manages the signals
void signalHandler(int signal){
	//Checks the signal type and deals with it appropriately
	if (signal == SIGINT){
		printf("\nParent : received SIGINT");
		printf("\nClosing down the program!\n");

		*s++ = '$';
		sleep(1);

		if (shmdt (shm) < 0) { 
			perror ("just can't let go\n"); 
			exit (1); 
		} 
		if (shmctl (shmid, IPC_RMID, 0) < 0) { 
			perror ("can't deallocate\n"); 
			exit(1); 
		}

		exit (0);
	}
}
