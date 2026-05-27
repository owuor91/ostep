#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sched.h>
#include <mach/mach.h>
#include <mach/thread_policy.h>

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

void pin_to_cpu(int cpu_id) {
    thread_affinity_policy_data_t policy = { cpu_id };
    thread_policy_set(
        mach_thread_self(),
        THREAD_AFFINITY_POLICY,
        (thread_policy_t)&policy,
        THREAD_AFFINITY_POLICY_COUNT
    );
}


/*
In this program the parent process creates a child process with fork()
There are 2 pipes created as well so the parent and child send each other the same byte over and over along
the pipes triggerring 2 context switches each time between them
*/

void measure_context_switch_cost(){
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);                             // Create 2 pipes 

    int iterations = 100000;
    char byte = 'x';

    pin_to_cpu(1);

    pid_t pid = fork();

    if(pid==0){
        for(int i = 0; i< iterations; i++){         // Inside the child process read waits to receive a byte from pipe 1 then sends back a byte on pipe 2.       
            read(pipe1[0], &byte, 1);               // This read blocks the process until the parent writes, triggering a context switch to parent
            write(pipe2[1], &byte, 1);
        }
        _exit(0);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for(int i=0; i< iterations; i++){           // In the parent process it writes a byte to pipe 1 which unblocks the child process. Triggering a context switch to child
        write(pipe1[1], &byte, 1);
        read(pipe2[0], &byte, 1);              // Child then writes to pipe2, unblocking parent and triggering context switch back to parent
    }

    gettimeofday(&end, NULL);

    long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);  // Calculate total elapsed time in ms
    double avg_cost = (double)elapsed_time/(2.0 * iterations);      // Divide total elapsed time by (iterations x 2 ) because each iteration entails 2 context switches
    printf("Total elapsed time: %ld microseconds over %d iterations\n", elapsed_time, iterations);
    printf("Average context switch cost %.2f microseconds \n", avg_cost);
}


int main(){
    measure_context_switch_cost();
    return 0;
}

