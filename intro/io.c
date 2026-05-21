#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    int fd = open("/tmp/file", O_WRONLY | O_CREAT | O_TRUNC | S_IRWXU);
    assert(fd > -1); //crash if file isn't created

    int rc = write(fd, "hello world\n", 12);
    assert(rc ==12);
    close(fd);
    return 0;
}

/*
Creates a file descriptor (an int that holds a reference to a file) for a file "/tmp/file"
with some flags to: (O_WRONLY) open it only for writes, (O_CREAT) create it if it doesn't exist,
(O_TRUNC) wipe its contents if it already exists and (S_IRWXU) assign read write and execute 
permissions to the owner. 
Then writes "hello world\n" (12 bytes) to it then returns the number of bytes written to confirm
that everything has been written. Finally close the file and exit the program. 
Demonstrates persistence by being able to "cat /tmp/file" after the program exits
*/