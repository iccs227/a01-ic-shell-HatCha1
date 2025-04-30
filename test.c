#include <stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>

void handler(int Sig){}

int main()
{
    sigset_t sigint, oldmask; sigemptyset(&sigint); sigaddset(&sigint, SIGINT);
    sigprocmask(SIG_BLOCK, &sigint, &oldmask);

    pid_t c=fork();
    if(0>c) return perror(0),1;
    if (c==0){
        signal(SIGINT, handler);
        sigdelset(&oldmask,SIGINT); /*in (the unlikely) case the process started with SIGINT blocked*/
        sigsuspend(&oldmask);
        printf("signal was given\n");
        exit(0);
    }
    kill(c,SIGINT);
    wait(0);
    return 0; 
}