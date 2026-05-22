#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
    printf("hello (pid: %d)\n", getpid());
    int rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(rc==0){
        printf("child (pid: %d)\n", (int) getpid());
    }else{
        int rc_wait = wait(NULL);
        printf("parent of %d (rc_wait: %d) (pid: %d)", rc, rc_wait, getpid());
    }
    return 0;
}


/*
This is identical to p1 with the exception of wait()
p1 is non-deterministic. The parent process runs and exits before the child is done and we end up with

    hello (pid: 97392)
    parent of 97393 (pid: 97392)
    child (pid: 97393)

in p2 the wait() call makes the parent wait for the child to finish before it continues. therefore we get

    hello (pid: 97686)
    child (pid: 97687)
    parent of 97687 (rc_wait: 97687) (pid: 97686)

child always completes before parent
*/