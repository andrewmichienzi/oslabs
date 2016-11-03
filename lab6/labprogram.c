#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <errno.h>

#define SIZE 16
/*
typedef struct {
        int value
        struct process *list
} semaphore;
*/
int main (int argc, char* argv[]) 
{ 
   int status; 
   long int i, loop, temp, *shmPtr; 
   int shmId; 
   pid_t pid;
   int semId;
   
   

    //Initialize semaphore
    semId = semget(IPC_PRIVATE, 1, 00600);
    
    
    //initialize the semaphore set referenced by the previously obtained semId handle
    semctl(semId, 0, SETVAL, 1);
 
 	//Structure of wait and signal
	struct sembuf wait, signal;
   
    //Define wait structure
	wait.sem_num = 0;
	wait.sem_op = -1;
	wait.sem_flg = SEM_UNDO;

	//Define signal structure
	signal.sem_num = 0;
	signal.sem_op = 1;
	signal.sem_flg = SEM_UNDO;
   
    
      // get value of loop variable (from command-line argument)
      if (argc != 2)
	{
		printf("./a.out [number of loops] \n");
		exit(1);
	}
	
	loop = atoi(argv[1]);

   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n"); 
      exit (1); 
   } 
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) { 
      perror ("can't attach\n"); 
      exit (1); 
   }

   shmPtr[0] = 0; 
   shmPtr[1] = 1;

   if (!(pid = fork())) { 
      for (i=0; i<loop; i++) { 
               // swap the contents of shmPtr[0] and shmPtr[1] 		
               //Wait
			semop(semId, &wait, 1);
            
            temp = shmPtr[0];
		    shmPtr[0] = shmPtr[1];
		    shmPtr[1] = temp;
		    
		    //signal
		    semop(semId, &signal, 1);
      } 
      if (shmdt (shmPtr) < 0) { 
         perror ("just can't let go\n"); 
         exit (1); 
      } 
      exit(0); 
   } 
   else 
      for (i=0; i<loop; i++) { 
        // swap the contents of shmPtr[1] and shmPtr[0] 
		//Wait	
		semop(semId, &wait, 1);
		
		temp = shmPtr[0];
		shmPtr[0] = shmPtr[1];
		shmPtr[1] = temp;
	    
	    //signal
        semop(semId, &signal, 1);  
      }

   waitpid(pid, &status, 0);
   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   if (shmdt (shmPtr) < 0) { 
      perror ("just can't let go\n"); 
      exit (1); 
   } 
   if (shmctl (shmId, IPC_RMID, 0) < 0) { 
      perror ("can't deallocate\n"); 
      exit(1); 
   }

   return 0; 
} 

/* 	./a.out 1000000
	values: 0     1 */
