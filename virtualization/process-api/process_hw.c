#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>


void fork_x(){
    int x = 100;
    int rc = fork();
    if(rc<0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if (rc==0){
        printf("child x before %d\n", x);
        x = 105;
        printf("child x: %d\n", x);
    }else{
        wait(NULL);
        printf("parent x before %d\n", x);
        x = 110;
        printf("parent x: %d\n", x);
    }
    printf("final x: %d\n", x);
}


void fork_file(){
    int fd = open("./myfile", O_CREAT | O_WRONLY | O_TRUNC , S_IRWXU);
    int rc = fork();
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if (rc==0){
        char *child_text = "Child file contents\n";
        write(fd, child_text, strlen(child_text));
        close(fd);
    }else
    {
        wait(NULL);
        char *parent_text = "Parent file contents\n";
        write(fd, parent_text, strlen(parent_text));
        close(fd);
    }
    
}

// waitpid() is useful where the parent process has multiple children and we want to specify which one to wait for

void fork_hello(){
    int rc = fork();
    if(rc<0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(rc==0){
        printf("hello\n");
    }else{
        waitpid(rc, NULL,0);  
        printf("goodbye\n");
    }
}

void fork_ls(){
    int rc = fork();
    if(rc<0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(rc==0){
        char *my_args[2];
        my_args[0] = strdup("/bin/ls");
        my_args[1] = NULL;
        execv(my_args[0], my_args);
        fprintf(stderr, "exec failed\n");
        exit(1);
    }else{
        int wait_rc = wait(NULL);
        printf("Back to parent process, returned: %d, child pid: %d parent pid: %d\n", wait_rc, rc, getpid());
    }
}


void close_stdout(){
    int rc = fork();
    if (rc<0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if (rc==0){
        close(STDOUT_FILENO);
        printf("Attempting to print out in child process\n");
    }else{
        wait(NULL);
        printf("Printing out in parent process\n");
    } 
}

void pipe_children(){
    int pipefd[2];                    //create array of 2 ints that will hold the file descriptors created by pipe
    if(pipe(pipefd)<0){               
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    /*
    pipe(pipefd) creates a pipe and populates the pipefd array with 2 file descriptors.
    pipefd[0] is the read end while pipefd[1] is the write end.
    Data written to pipefd[1] can be read from pipefd[0]

    */

    int rc1 = fork();
    if(rc1<0){ fprintf(stderr, "fork failed\n"); exit(1); }

    if(rc1==0){
        close(pipefd[0]);                  //close read end of the pipe, not needed as this child writes to pipe
        dup2(pipefd[1], STDOUT_FILENO);    //standard out writes to pipe. pipefd[1] is the write end
        close(pipefd[1]);                  //close pipe after writing to it so that EOF is sent and read() below doesn't hang forever waiting for EOF

        printf("hello from child 1, pid:%d\n ", getpid());
        exit(0);
    }

    int rc2 = fork();
    if(rc2<0){ fprintf(stderr, "fork failed\n"); exit(1); }

    if(rc2==0){
        close(pipefd[1]);                 //close write end of the pipe, not needed as this process reads from pipe
        dup2(pipefd[0], STDIN_FILENO);    //standard in reads from pipe  pipefd[0] is the read end
        close(pipefd[0]);                 //close pipe after reading so that EOF gets sent and read wont hang

        char buf[128];                                    //create a 128 byte buffer to hold what we are going to read
        int n = read(STDIN_FILENO, buf, sizeof(buf)-1);   // read upto 127 bytes from STD_IN which is the pipe leave 1 byte for the null terminator
        buf[n] = '\0';                                    //null terminate the buffer to make it a valid C string 
        printf("child 2 (pid: %d) received: %s\n", getpid(), buf);
        exit(0);
    }


    close(pipefd[0]);                              //parent closes both ends of the pipe
    close(pipefd[1]);

    waitpid(rc1, NULL,0);                          
    waitpid(rc2, NULL,0);                          //waits for each child by its pid
    printf("Back to parent, pid: %d\n", getpid());    
}

int main(int argc, char *argv[]){
    pipe_children();
    return 0;
}

