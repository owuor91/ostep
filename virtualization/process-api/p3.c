#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
    printf("hello (pid: %d)\n", getpid());
    int rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed \n");
        exit(1);
    }else if(rc ==0){
        printf("child (pid: %d)", getpid());

        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("p3.c");
        myargs[2]= NULL;

        execvp(myargs[0], myargs);
        printf("This should not print out");
    }else{
        int rc_wait = wait(NULL);
        printf("parent of %d (rc_wait: %d) (pid: %d)\n", rc, rc_wait, getpid());
    }
    return 0;
}

/*
Similar to p2 only that iin this case the child process (rc==0) creates a new argument vector to run word count
on the same file then invokes wc using execvp(). execvp() overrides the current code segment and reinitializes
the heap and stack of the program, effectively transforming the current program into another one (wc).
Since a successful call to execvp never returns line 23 doesn't execute. 
exec is used to run a different program within the current process as opposed to fork() which runs a copy of the same program

The application of fork(), exec() and wait() is for example in the shell when a user types a command (name of an executable program + arguments):
1. shell figures out where to find the executable in the file system, 
2. calls fork() to start a child process within which to run the command
3. calls exec() to run the command,(a different program)
4. calls wait() to wait for the command to complete
5. when the command completes, shell returns from wait() and prints out a prompt waiting for the next command
*/