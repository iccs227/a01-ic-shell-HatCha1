#include "job.h"

Job jobs[MAX_JOBS];
int jobCount = 0;
int nextJobId = 1;
pid_t shell_pgid;

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
void cmdFg(char* buffer){
    char* temp = strchr(buffer, '%');
    memmove(temp, temp+1, strlen(temp)); //Get Job ID
    int jobId = atoi(temp);

    for (int i = 0; i < jobCount; i++) {
        if (jobs[i].id == jobId) {
            pid_t pid = jobs[i].pid;

            kill(pid, SIGCONT);

            tcsetpgrp(0, pid);
            printf("%s\n", jobs[i].command);
            fflush(stdout);
            int stat;
            waitpid(pid, &stat, WUNTRACED); 

            tcsetpgrp(0, shell_pgid);

            if (WIFSTOPPED(stat)) {
                jobs[i].running = false;
                printf("\n[%d] Stopped\t%s\n", jobs[i].id, jobs[i].command);
            } else {

                for (int j = i; j < jobCount - 1; j++) {
                    jobs[j] = jobs[j + 1];
                }
                jobCount--;
            }

            if (WIFEXITED(stat)) {
                prevExitCode = WEXITSTATUS(stat);
            } else {
                prevExitCode = 1;
            }

            return;
        }
    }
}
void cmdBg(char* buffer) {
    char* temp = strchr(buffer, '%');
    memmove(temp, temp+1, strlen(temp)); //Get Job ID
    int jobId = atoi(temp);

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
}

Job createJob(char* command, pid_t pid){
    Job job;
    job.id = nextJobId++;
    job.pid = pid;
    strncpy(job.command, command, MAX_CMD_BUFFER);
    job.running = true;
    job.done = false;
    jobs[jobCount++] = job;
    return job;
}