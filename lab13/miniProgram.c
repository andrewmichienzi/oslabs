#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("Dude..... you need 4 arguments\n");
		printf("./a.out [link] [target] [destination]\n");
		exit(1);
	}	  

	if(strcmp(argv[1], "-s") == 0)
	{
		symlink(argv[3], argv[2]);
		printf("Soft linked\n");
	}

	if(strcmp(argv[1], "-h") == 0)
	{
		link(argv[3], argv[2]);
		printf("Hard linked\n");	
	}
	return 0;
} 
