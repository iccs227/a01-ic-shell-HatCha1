#include "executor.h"
/**
 * @name runExternal
 * @brief Run external command with child process while handling I/O redirection
 * @param buffer original user input
 * @param argv arguments from bufferToArg function
 * @param argc number of arguments
 * @param in File name for input redirection
 * @param out File name for output rediection
 * @returns void
 */
void runExternal(char* buffer, char* argv[], int argc, char* in, char* out) {
    int ppid = getpid(); // Parent group process id

    bool bg = checkAmpersand(argv, argc); // Check if the command should be run in bg or not
    
    pid_t pid;
    if ((pid=fork()) < 0){ // Error when fork
        perror ("Fork failed");
        prevExitCode = 1;
        return;
    }
    if (!pid) { // Child process section
        
        setpgid(0, 0);  // Child creates a new process group
        
        if (!bg) {
            tcsetpgrp(0, getpid()); // If not bg, then give terminal to child
        }

        signal(SIGINT, SIG_DFL); // Return default signal to child process
        signal(SIGTSTP, SIG_DFL);

        if (in != NULL){ // Handling input redirection
            int fd = open(in, O_RDONLY);
            if (fd < 0){
                perror("No Such File");
                exit(1);
            }
            dup2(fd, 0); // Getting input from the file
        }
        if (out != NULL){ // Handling output redirection
            int fd = open(out, O_TRUNC | O_CREAT | O_WRONLY, 0666);
            dup2(fd, 1); // Change output direction to this file
        }
        execvp(argv[0], argv); // Execute the command
        perror("");
        exit(1);
        
    }
    else { // Parent process section
        if (bg){ // If it is a bg process, then create a job 
            Job job = createJob(buffer, pid, true);

            printf("[%d] %d\n", job.id, job.pid);

            fflush(stdout);
            prevExitCode = 0;
            return;
        }
        tcsetpgrp(0, pid); // Give terminal to child process

        int stat;
        waitpid(pid, &stat, WUNTRACED); // Wait until the child process is done
        tcsetpgrp(0, ppid); // Give terminal back to parent process

        if (WIFSTOPPED(stat)) { // If sigtstp is signaled, then create another job
            Job job = createJob(buffer, pid, false);

            printf("\n[%d] Stopped\t%s\n", job.id, buffer);
        }
        if (WIFEXITED(stat)) { // Exit in good condition
            prevExitCode = WEXITSTATUS(stat);
        } else { // Not good condition
            prevExitCode = 1;
        }
    }
}