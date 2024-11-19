#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>

/**
 * Function: my_fork
 * -----------------
 * Calls fork() to create a new process.
 * 
 * If fork() succeeds (its return value is non-negative), 
 * the function returns the value returned by fork().
 * 
 * If fork() fails (its return value is negative), 
 * the function prints an error message and exits the process.
 * 
 * Returns:
 *  pid_t: The process ID of the child process to the parent, 
 *         and 0 to the child process.
 */

pid_t my_fork(void){
    pid_t pid = fork();
    if (pid < 0) {
        perror ("Fork failed");
        exit(1);
    }
    return pid;
}

/**
 * Function: print_pids
 * --------------------
 * Prints the process IDs and their generations to the specified file descriptor.
 * 
 * Parameters:
 *  fd - file descriptor to which the output should be written
 *  N  - number of iterations each process should perform
 *  G  - maximum generation of processes
 */
void print_pids(int fd, short unsigned int N, short unsigned int G) {
    int generation = 0;
    for (int i = 0; i < N; i++) {
        if (generation < G) {
            pid_t my_pid = my_fork();
            if (my_pid == 0) {
                // Child process
              generation++;
              i = -1;
              continue;
            }
            else wait(NULL);
        }    
                
        
    }
    dprintf(fd, "My pid is %d. My generation is %d.\n", getpid(), generation);
    exit(0); // Exit child process       
}

int main(void) {
    int fd = STDOUT_FILENO; // File descriptor (using standard output for this example)
    short unsigned int N = 2; // Number of iterations each process should perform
    short unsigned int G = 3; // Maximum generation of processes

    print_pids(fd, N, G);

    return 0;
}