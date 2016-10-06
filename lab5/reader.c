#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    int shmid;
    key_t key;
    char *shm, *s;

    //Name for the shared memory segment
    key = 6000;

    //Accesses the shared memory segment
    if ((shmid = shmget(key, 25, 0)) < 0) {
        perror("shmget");
        exit(1);
    }

    //Attach segment to address space
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

	while(1){

		//Reads from memory until terminating character
		s = shm;

		if(*s == '$'){
			printf("Shutting Down!\n");
			return(0);
		}

		for (s; *s != '\0'; s++){
		    putchar(*s);
		}
		putchar('\n');

		//This ensures that both readers read from memory
		if(*shm != '*'){
			*shm = '*';
			while(*shm == '*')
				;
		}else if(*shm == '*'){
			*shm = '&';
		}

		//Loops until the writer updates the data in shared memory
		while(*shm == '&'){
			sleep(1);
		}
	}
    return 0;
}
