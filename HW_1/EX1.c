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
    pid_t pid = fork(); // Call fork()
    if (pid < 0) {    // Check if fork() failed
        perror ("Fork failed");
        exit(1);        // Exit process
    }
    return pid; // Return process ID
}

/**
 * For each g=0, 1, ..., G, the function should print N_g lines in the following format:
 * My pid is p. My generation is g.
 * 
 * Where:
 *  - g is the generation of the process: the generation of the original (first) process is 0.
 *    If the generation of a process is i, then the generation of its child process is i+1.
 *  - p is the pid of the process which prints the line.
 * 
 * For instance, the line may be: My pid is 1038. My generation is 0.
 * 
 * Note that the pids are arbitrary, and are dependent upon the processes’ scheduling on your machine.
 * In particular, the operating system may allocate a new process the same pid of an old terminated process.
 * The function should print its output to the file, whose file descriptor is the input fd.
 * 
 * Limitations and hints:
 *  - The function should use a single loop.
 *  - Each process iterates exactly N times over the loop.
 *  - Each process prints a single line.
 *  - Use my_fork(), not fork().
 *  - No prints nor calls to my_fork() are allowed outside the loop.
 *  - Each process should print its line only after all its children printed their lines.
 */

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
    int generation = 0;                 // Initialize generation variable
    for (int i = 0; i < N; i++) {
        if (generation < G) {
            pid_t my_pid = my_fork();
            if (my_pid == 0) {
                // Child process
              generation++; // Increment generation
              i = -1;    // Reset loop counter
              continue; // Restart loop
            }
            else wait(NULL);    // Parent process - wait for child process to finish
        }    
    }
    dprintf(fd, "My pid is %d. My generation is %d.\n", getpid(), generation);  // Print process ID and generation
    exit(0); // Exit child process 
}

int main(void) {
    int fd = STDOUT_FILENO; // File descriptor (using standard output for this example)
    short unsigned int N = 3; // Number of iterations each process should perform
    short unsigned int G = 2; // Maximum generation of processes

    print_pids(fd, N, G);

    return 0;
}