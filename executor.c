#include "executor.h"

void runExternal(char* buffer) {
    char* argv[64];
    char copyBuffer[MAX_CMD_BUFFER];
    char* in = NULL;
    char* out = NULL;
    int ppid = getpid();

    bool bg = checkAmpersand(&buffer);
    strncpy(copyBuffer, buffer, MAX_CMD_BUFFER);
    bufferToArg(copyBuffer, argv, &in, &out);

    pid_t pid;
    if ((pid=fork()) < 0){ //Error
        perror ("Fork failed");
        prevExitCode = 1;
        return;
    }
    if (!pid) { //Child
        
        setpgid(0, 0);  // Child creates a new process group
        
        if (!bg) {
            tcsetpgrp(0, getpid()); // Give terminal to child if foreground
        }

        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        if (in != NULL){
            int fd = open(in, O_RDONLY);
            if (fd < 0){
                perror("No Such File");
            }
            dup2(fd, 0);
        }
        if (out != NULL){
            int fd = open(out, O_TRUNC | O_CREAT | O_WRONLY, 0666);
            dup2(fd, 1);
        }
        execvp(argv[0], argv);
        perror("");
        exit(1);
    }
    else { //Parent
        setpgid(pid, pid);

        if (bg){
            Job job = createJob(buffer, pid);

            printf("[%d] %d\n", job.id, job.pid);

            fflush(stdout);
            prevExitCode = 0;
            return;
        }
        tcsetpgrp(0, pid);

        int stat;
        waitpid(pid, &stat, WUNTRACED);
        tcsetpgrp(0, ppid);

        if (WIFSTOPPED(stat)) { //https://linux.die.net/man/3/waitpid
            Job job = createJob(buffer, pid);
            job.running = false;

            printf("\n[%d] Stopped\t%s\n", job.id, argv[0]);
        }
        if (WIFEXITED(stat)) {
            prevExitCode = WEXITSTATUS(stat);
        } else {
            prevExitCode = 1;
        }
    }
}