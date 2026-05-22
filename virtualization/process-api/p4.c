#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
    int rc = fork();

    if(rc <0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(rc==0){
        close(STDOUT_FILENO);
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC | S_IRWXU);

        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("p4.c");
        myargs[2] = NULL;

        execvp(myargs[0], myargs);
    }else{
        int rc_wait = wait(NULL);
    }

    return 0;
}

/*
Creates a new process within which standard out is closed then a new file p4.output is opened.
When execvp() runs wordcount on p4.c it prints its output to p4.output. 
Parent process meanwhile waits for child to complete.

This demontrates how the output of one process can be piped to the input of another process.
This is implemented in UNIX systems using the pipe system call e.g
grep 'process' p4.c | wc -l ------> 4

*/