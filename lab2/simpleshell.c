#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define SIZE 50
int main()
{
  char *datain;
  datain = malloc(SIZE);

  printf("\n\n");
  printf(">>");
  fgets(datain, SIZE, stdin);
  printf("%s", datain);

  int i;
  char* command[50];
  for(i = 0; i < 50; i++){
    command[i] = strsep(&datain, " ");
  }
  

  pid_t pid, child;
  int status;

  if((pid=fork())<0){
    perror("fork failure");
    exit(1);
  }

  if(pid==0){
    //child
    if(execvp(command[0], &command[0])<0){
      perror("exec failed");
      exit(1);
    }
  }

  else{
    //Parent
    child = wait(&status);
  }
  printf("%s",command[0]);
  printf("\n");
  printf("%s",command[3]);
  return (0);
}

