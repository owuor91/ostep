#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include "./common.h"


// argc - Argument count: Number of arguments passed to the program including program name
// argv - Argument vector: Array containing proram name and all arguments passed
// checks if exactly 2 arguments have been passed then assigns arg string argv[1] to *str
// before looping infinitely to check the time every second and print str
// running multiple instance with ./cpu A & ./cpu B & ./cpu C & ... demonstrates cpu virtualization 
int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "usage: cpu <string>\n");
        exit(1);
    }
    char *str = argv[1];
    while (1)
    {
        Spin(1);
        printf("%s\n", str);
    }
    return 0;
}