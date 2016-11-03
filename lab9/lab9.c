#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static char *initializedData = "cats";
static char *uninitializedData;

int main (int argc, char *argv[]){
    char * heapVar = (char *)malloc(500);
    int var = 12;
    int stack[100];

    printf("\nInitialized Data : %p\n", &initializedData);
    printf("Uninitialized Data : %p\n", &uninitializedData);

    printf("\nHeap Start : %p\n", sbrk(0));
    printf("Heap Variable Start : %p\n", &heapVar[0]);
    printf("Heap Variable End : %p\n", &heapVar[499]);

    printf("\nStack Start : %p\n", &var);
    printf("Stack[0] : %p\n", &stack[0]);
    printf("Stack[99] : %p\n", &stack[79]);

    printf("\nEnvironmental Data : %p\n", &argv[0]);

    void* main_ptr = main;
    printf("Main pointer = %p\n", main_ptr); 
    pause();

    return 0;
}

