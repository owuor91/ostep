#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

void measure_system_call_cost(){
    int fd = open("/dev/null", O_RDONLY);
    char buf[1];
    int iterations = 1000000;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for(int i = 0; i< iterations; i++){
        read(fd, buf, 0);
    }

    gettimeofday(&end, NULL);

    long spent_microseconds = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    double avg_cost = (double)spent_microseconds / iterations;
    
    printf("Used %ld microseconds over %d iterations\n", spent_microseconds, iterations);

    printf("Average cost per syscall: %.2f microseconds\n", avg_cost);

    close(fd);
}

int main(){
    measure_system_call_cost();
    return 0;
}

