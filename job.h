#ifndef JOB_H
#define JOB_H

#include "globals.h"

typedef struct {
    int id;
    int pid;
    char command[MAX_CMD_BUFFER];
    bool running;
    bool done;
} Job;

extern Job jobs[MAX_JOBS];
extern int jobCount;
extern int nextJobId;
extern pid_t shell_pgid;

void cmdJobs();
void cmdFg(char* buffer, int argc);
void cmdBg(char* buffer, int argc);
Job createJob(char* command, pid_t pid);
void removeJob(int index);

#endif