#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
/*
 * Writer
 */
void signalHandler(int signal);

char * s;
struct Args * args;
int shmid;

struct Args{
	char * str;
	int writer;
	int reader1;
	int reader2;
};
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
    if ((shmid = shmget(key, 25, IPC_CREAT| 0777)) < 0) {
        perror("Error creating segment");
        exit(1);
    }

    //Attach segment to address space
    if ((args = (struct Args *) shmat(shmid, NULL, 0)) == (struct Args *) -1) {
        perror("Error attaching segment");
        exit(1);
    }


	//Main memory loop
	while(1){
		(*args).writer = 1;
		(*args).reader1 = 0;
		(*args).reader2 = 0;
		printf("1\n");
		//Get user input		
		//s = shm;
		int length;
		fgets(userIn, sizeof(userIn), stdin);
		length = strlen(userIn);
		printf("%d\n", length);
		//Set segment memory to user input
		int i;
		userIn[length] = '\0';
		for (i=0; i < length; i++)
		{
		    *((*args).str + i) = putchar(userIn[i]);
		}
		
		
		//Add terminating character at the end of user input
		//*(s+length) = '\0';
		printf("%s\n", (*args).str);

		//This checks if both readers have read from memory
		while ((*args).writer == 0)
		{
		    printf("5\n");
		    sleep(1);
		}
		(*args).reader1 = 0;
		(*args).reader2 = 0;
	}

	if (shmdt (args) < 0) { 
		perror ("just can't let go\n"); 
		exit (1); 
	} 
	if (shmctl (shmid, IPC_RMID, 0) < 0) { 
		perror ("can't deallocate\n"); 
		exit(1); 
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

		if (shmdt (args) < 0) { 
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
