#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(int argc, char *argv[]){
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    printf("(%d) address pointed to by p: %p\n", getpid(), p);
    *p = 0;

    while(1){
        Spin(1);
        *p = *p + 1;
        printf("(%d) p: %d\n", getpid(), *p);
    }
    return 0;
}

/*
Allocates some memory for an integer and prints out its address
Then assigns a value 0
Starts a loop that checks time every second then increments the value stored in p before printing it
When 2 or more instances run they all point to the same memory address yet they seem to be independently
manipulating the value without interfering with one another
This is because of memory virtualization. The OS maps the same virtual memory address to different
physical memory locations for each process (instance of the program)
*/