#include "signal.h"

void sig_done(int signum){
    pid_t pid;
    int status;

    while((pid = waitpid(-1, &status, WNOHANG)) > 0) { // -1 == every child, waitpid > 0 == finish
        for (int i = 0; i < jobCount; i++){
            if (jobs[i].pid == pid){
                if (WIFEXITED(status)) {
                    jobs[i].running = false;
                    jobs[i].done = true;
                    printf("\n[%d]+ Done                    %s\n", jobs[i].id, jobs[i].command);
                    printf("icsh $ ");
                    fflush(stdout);
                    for (int j = i; j < jobCount - 1; j++) { // Shifting
                        jobs[j] = jobs[j + 1];
                    }
                    jobCount--;
                }
                break;
            }
        }
    }
}