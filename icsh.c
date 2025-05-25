/* ICCS227: Project 1: icsh
 * Name: Pasu Kositanont
 * StudentID: 6680060
 */

#include "globals.h"
#include "built_in.h"
#include "executor.h"
#include "parser.h"
#include "signal.h"
#include "command.h"

int main(int argc, char* argv[]) {

    char buffer[MAX_CMD_BUFFER];

    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    
    signal(SIGCHLD, sig_done);

    shell_pgid = getpid();
    setpgid(shell_pgid, shell_pgid);
    tcsetpgrp(0, shell_pgid);

    if (argc > 1){
        script = true;
        FILE* file = fopen(argv[1], "r");

        if (file == NULL){
            printf("No such file %s\n", argv[1]);
            return 0;
        }

        while (fgets(buffer, sizeof(buffer), file)) {

            buffer[strcspn(buffer, "\n")] = '\0';
            allCommands(buffer);
        }

        fclose(file);
        return 0;
    }
    printf("Starting IC shell\n");

    while (1) {
        printf("icsh $ ");
        fgets(buffer, 255, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        allCommands(buffer);
    }
}