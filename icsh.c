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

    // Ignoring signals based on the assignment requirement
    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    
    // When child status has changed go to sig_done
    signal(SIGCHLD, sig_done);

    // Saving shell process group id for later use in fg
    shell_pgid = getpid();
    setpgid(shell_pgid, shell_pgid);
    tcsetpgrp(0, shell_pgid);

    // Check for script mode
    if (argc > 1){
        script = true;
        FILE* file = fopen(argv[1], "r");

        if (file == NULL){
            printf("No such file %s\n", argv[1]);
            return 0;
        }

        while (fgets(buffer, sizeof(buffer), file)) { // Read file line by line and run the command until the end

            buffer[strcspn(buffer, "\n")] = '\0';

            if (buffer[0] != '!' && buffer[1] != '!') { // If first 2 letters are !!, then copy buffer to prevBuffer

                char* temp = strchr(buffer, '>');
                char copyBuffer[MAX_CMD_BUFFER];
                strncpy(copyBuffer, buffer, MAX_CMD_BUFFER);
    
                if (temp != NULL){ // If > is shown in the buffer, then end the copyBuffer right before ">" and copy it to prevBuffer.
                    copyBuffer[strlen(buffer) - strlen(temp)] = '\0';
                }
                strncpy(prevBuffer, copyBuffer, MAX_CMD_BUFFER);
            }

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

        if (buffer[0] != '!' && buffer[1] != '!') { // If first 2 letters are !!, then copy buffer to prevBuffer

            char* temp = strchr(buffer, '>');
            char copyBuffer[MAX_CMD_BUFFER];
            strncpy(copyBuffer, buffer, MAX_CMD_BUFFER);

            if (temp != NULL){ // If > is shown in the buffer, then end the copyBuffer right before ">" and copy it to prevBuffer.
                copyBuffer[strlen(buffer) - strlen(temp)] = '\0';
            }
            strncpy(prevBuffer, copyBuffer, MAX_CMD_BUFFER);
        }
        allCommands(buffer); // Run commands
    }
}