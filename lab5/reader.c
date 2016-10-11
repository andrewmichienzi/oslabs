#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
struct Args * args;
int main()
{
    int shmid;
    key_t key;
    char *shm, *s;
	
	struct Args{
		char * str;
		int writer;
		int reader1;
		int reader2;
	};

    //Name for the shared memory segment
    key = 6000;

    //Accesses the shared memory segment
    if ((shmid = shmget(key, 25, IPC_CREAT | 0777)) < 0) {
        perror("shmget");
        exit(1);
    }

    //Attach segment to address space
    if ((args = (struct Args *)shmat(shmid, NULL, 0)) == (struct Args *) -1) {
        perror("shmat");
        exit(1);
    }

	while(1){
`		while(args1.writer)
		{
			sleep(1);
		}
		if((*args).reader == 0)
		{
			(*args).reader1 = 1;
		}
		else{
			while((*args).reader2 == 0)
				sleep(1);
		}
		//Reads from memory until terminating character
		s = (*args).str;
/*
		if(*s == '$'){
			printf("Shutting Down!\n");
			return(0);
		}
*/
		for (s; *s != '\0'; s++){
		    putchar(*s);
		}
		putchar('\n');

		//This ensures that both readers read from memory
		if((*args).reader2 == 1)
		{
			(*args).writer = 1;
		}	
	}
    return 0;
}
