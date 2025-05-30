#include "job.h"

Job jobs[MAX_JOBS];
int jobCount = 0;
int nextJobId = 1;
pid_t shell_pgid;

/**
 * @name cmdJobs
 * @brief print out jobs that are currently running or stopped.
 * @param None
 * @returns void, only print  
 */

void cmdJobs(){
    for (int i = 0; i < jobCount; i++){
        if (jobs[i].running == true) {
            printf("[%d]  Running                 %s\n", jobs[i].id, jobs[i].command);
        }
        else {
            printf("[%d]  Stopped                 %s\n", jobs[i].id, jobs[i].command);
        }
    }
}

/**
 * @name cmdFg
 * @brief Bring background processes to foreground and run
 * @param buffer argv[1] e.g. %1
 * @param argc number of arguments
 * @returns void, only bring
 */

void cmdFg(char* buffer, int argc){

    if (argc != 2){ // If not what we wanted
        printf("Example fg command: 'fg %%1'\n");
        return;
    }

    memmove(buffer, buffer+1, strlen(buffer)); //Get Job ID
    int jobId = atoi(buffer);

    for (int i = 0; i < jobCount; i++) {
        if (jobs[i].id == jobId) {
            pid_t pid = jobs[i].pid;

            kill(pid, SIGCONT); // Continue the process

            tcsetpgrp(0, pid); // Give terminal to child process
            printf("%s\n", jobs[i].command);
            fflush(stdout);
            int stat;
            waitpid(pid, &stat, WUNTRACED); // Wait for the child to finish

            tcsetpgrp(0, shell_pgid);

            if (WIFSTOPPED(stat)) { // If the child is stopped
                jobs[i].running = false;
                printf("\n[%d] Stopped\t%s\n", jobs[i].id, jobs[i].command);
            }
            else { // If not stopped then remove that job
                removeJob(i); 
            }

            // Update exit code
            if (WIFEXITED(stat)) {
                prevExitCode = WEXITSTATUS(stat);
            } else {
                prevExitCode = 1;
            }

            return;
        }
    }
}

/**
 * @name cmdBg
 * @brief Bring stopped processes to run in background
 * @param buffer argv[1] e.g. %1
 * @param argc number of arguments
 * @returns vonid, only brig
 */

void cmdBg(char* buffer, int argc) {

    if (argc != 2){ // If not what we wanted
        printf("Example fg command: 'bg %%1'\n");
        return;
    }

    memmove(buffer, buffer+1, strlen(buffer)); //Get Job ID
    int jobId = atoi(buffer);

    for (int i = 0; i < jobCount; ++i) {
        if (jobs[i].id == jobId) {
            kill(jobs[i].pid, SIGCONT);  // Resume the process

            jobs[i].running = true;      // Mark job as running
            printf("[%d]+ %s &\n", jobs[i].id, jobs[i].command);
            fflush(stdout);

            prevExitCode = 0;
            return;
        }
    }
    prevExitCode = 1; // Can't find the given job id
}

/**
 * @name createJob
 * @brief Create a job and put in the necessary details
 * @param command First argument
 * @param pid Process id that we are building for
 * @param isRunning Tell the function whether the process is stopped or still running
 * @returns a new Job in jobs list
 */
Job createJob(char* command, pid_t pid, bool isRunning){
    Job job;
    job.id = nextJobId++;
    job.pid = pid;
    strncpy(job.command, command, MAX_CMD_BUFFER);
    job.running = isRunning;
    job.done = false;
    jobs[jobCount++] = job;
    return job;
}

/**
 * @name removeJob
 * @brief Removing a job from jobs list
 * @param index Locate which job to remove
 * @returns void, just shifting a list
 */
void removeJob(int index){
    for (int j = index; j < jobCount - 1; j++) {
        jobs[j] = jobs[j + 1];
    }
    jobCount--;
    return;
}