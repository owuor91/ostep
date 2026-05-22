#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    printf("hello (pid: %d)\n", (int) getpid());
    int rc = fork();
    if(rc<0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if (rc==0){
        printf("child (pid: %d)\n", (int) getpid());
    }else{
        printf("parent of %d (pid: %d)\n", rc, (int) getpid());
    }
    
    return 0;
}

/*
When this program runs both the else if and else blocks run. Here's why?
fork() creates a new child process which behaves like an almost ecact replica of the parent process, 
running the same instructions as the parent.
Therefore at the point of creation the child process gets the return code 0 and executes the else if block
while the parent process gets the pid of the child process as the return code and executes the else block
*/