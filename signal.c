#include "signal.h"

/**
 * @name sig_done
 * @brief Signal handler when a child process is finish.
 * @param signum Signal identifier
 * @returns void, only print out the finish jobID  
 */

void sig_done(int signum){
    pid_t pid;
    int status;

    while((pid = waitpid(-1, &status, WNOHANG)) > 0) { // Check if any child(-1) is finish(> 0)
        for (int i = 0; i < jobCount; i++){
            if (jobs[i].pid == pid){
                if (WIFEXITED(status)) {
                    jobs[i].running = false;
                    jobs[i].done = true;
                    printf("\n[%d]+ Done                    %s\n", jobs[i].id, jobs[i].command);
                    printf("icsh $ ");
                    fflush(stdout);
                    
                    removeJob(i);
                }
                break;
            }
        }
    }
}