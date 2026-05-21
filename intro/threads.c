#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "common_threads.h"


volatile int counter = 0;
int loops;

void *worker(void *arg){
    int i;
    for(i=0; i< loops; i++){
        counter++;
    }
    return NULL;
}

int main(int argc, char *argv[]){
    if(argc !=2){
        fprintf(stderr, "pass a value like this: threads <value>\n");
        exit(1);
    }

    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value: %d\n", counter);

    Pthread_create(&p1, NULL, worker, NULL);
    Pthread_create(&p2, NULL, worker, NULL);
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("Final Value : %d\n", counter);
    return 0;
}

/*
Demonstrates threading problems by defining a worker to increment a counter
then creating 2 threads to run the worker. Expectation is that if each thread
loops 1000 times for example the final counter value should be 2000. 
However for larger loops values this isn't consistent due to threads overwriting
one another
*/