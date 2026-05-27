#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

void measure_system_call_cost(){
    int fd = open("/dev/null", O_RDONLY);   // open /dev/null a file that discards reads and returns nothing on read
    char buf[1];                            // declare a 1 byte buffer to read into from dev/null nothing will be written to it
    int iterations = 1000000;

    struct timeval start, end;              // declare 2 structs to hold start & end time in seconds and microseconds
    gettimeofday(&start, NULL);             // write start time into start

    for(int i = 0; i< iterations; i++){     // perform read syscall 1000000 times so as to get an accurate average
        read(fd, buf, 0);
    }

    gettimeofday(&end, NULL);

    long spent_microseconds = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);   //obtain elapsed time in microseconds
    double avg_cost = (double)spent_microseconds / iterations;
    
    printf("Used %ld microseconds over %d iterations\n", spent_microseconds, iterations);

    printf("Average cost per syscall: %.2f microseconds\n", avg_cost);

    close(fd);
}

int main(){
    measure_system_call_cost();
    return 0;
}

