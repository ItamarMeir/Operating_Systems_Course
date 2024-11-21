#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * Function: my_fork (1)
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
 * For each g=0, 1, ..., G, the function should print N^g lines in the following format:
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
 * Function: print_pids (2)
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
   if (generation != 0) exit(0); // Exit child process 
}

/**
 * Write the function: void count_lines (short unsigned int G).
 * 
 * This function counts the number of lines in the file out.txt (to which we will write output, as 
 * described below); and then prints to the screen output which follows the following format: 
 * Number of lines by processes of generation 2 is 9 
 * Number of lines by processes of generation 1 is 3 
 * Number of lines by processes of generation 0 is 1 
 * 
 * Limitations and hints: 
 *  - The function should use a single loop. 
 *  - The function should generate either G-1 or G new processes.
 *  - Each process should print a single line: the line referring to generation g should be 
 *    printed by a process of generation g. 
 *  - You may use the commands system(), grep and wc. 
 *  - The output should be written in decreasing generation order (as in the sample above). 
 */

/**
 * Function: count_lines (3)
 * ---------------------
 * Counts the number of lines in the file out.txt and prints the number of lines for each generation.
 * 
 * Parameters:
 *  G - maximum generation of processes
 */
void count_lines(short unsigned int G) {
    char command[256];
    unsigned int counter = 0, gen = 0;

    for(int i=0; i<G; i++){     
        pid_t pid = my_fork();      // Create a new process
        gen = i;                    // Set generation
        if (pid == 0){  
            // Child process
            gen = i+1;            // Increment generation
            continue;           // Continue loop
        }
        break;                  // The parent process should break the loop
    }
            wait(NULL); // Wait for the child to die
            sprintf(command, "count=$(grep 'My generation is %d.' out.txt | wc -l); \
                echo \"Number of lines by processes of generation %d is $count\"", gen, gen);   // Create command
                // Saves the number of lines in the file out.txt to the variable count
            system(command);                // Execute command
            if (gen != 0) exit(0);          // Exit child process
}

/**
 * Function: print_threads
 * -----------------------
 * Creates N threads, each of which prints a message indicating its thread number.
 * The order of the prints MUST appear in the correct order.
 * 
 * Parameters:
 *  N - the number of threads to create
 * 
 * Each thread will print: "Hi. I'm thread number i" where i is the i-th thread created.
 */

void print_threads(short unsigned int N){
    unsigned int thread_num;            // Thread number
    for(int i=N-1; i>=1; i--){         // Loop through threads
        pid_t pid = my_fork();        // Create a new process
        thread_num = i;              // Set thread number
        if (pid == 0){              
            // Child process
            thread_num = i-1;    // Decrement thread number
            continue;        // Continue loop
        }
        break;          // The parent process should break the loop
    }
    wait(NULL); // Wait for the child to die
    dprintf(STDOUT_FILENO, "Hi. I'm thread %d\n", thread_num);  // Print thread number
    if (thread_num != N-1) exit(0); // Exit children processes
}

int main(void) {
    //int fd = STDOUT_FILENO; // File descriptor (using standard output for this example)
    short unsigned int N = 3; // Number of iterations each process should perform
    short unsigned int G = 2; // Maximum generation of processes

    int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open file for writing, creating it if it doesn't exist,\
                                                                         and truncating it if it does. 0644 is the file permissions.
    if (fd == -1) {
        perror("open");
        return 1;
    }
    //print_pids(fd, N, G);   // Call task 2
    close(fd);  // Close file descriptor

    //count_lines(G); // Call task 3

    print_threads(10);
    return 0;
}

