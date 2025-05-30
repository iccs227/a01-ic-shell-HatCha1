#include "built_in.h"

/**
 * @name cmdEcho
 * @brief Repeat the input after the first argument
 * @param argv arguments that has echo as command
 * @returns void, only print
 */
void cmdEcho(char* argv[]){
    int i = 1;
    while (argv[i] != NULL){ // Print until the end of array
        printf("%s ", argv[i++]);
    }
    printf("\n");
}

/**
 * @name cmdBang
 * @brief Repeat the previous command
 * @param buffer the original user input
 * @returns void, run command recursively
 */
void cmdBang(char* buffer, char* argv[]){
    if (strlen(buffer) == 0){
        return;
    }

    char* temp = strchr(buffer, '>'); // Prepare for !! > a.txt
    char copyBuffer[MAX_CMD_BUFFER];

    if (temp != NULL){ // Getting rid of everything from > on wards
        strncpy(copyBuffer, buffer, MAX_CMD_BUFFER);
        copyBuffer[strlen(buffer) - strlen(temp)] = '\0';
    }

    allCommands(copyBuffer); // run allCommands recursively
}

/**
 * @name cmdExit
 * @brief Exit command with exit code
 * @param argv arguments that run the exit command
 * @param argc number of arguments
 * @returns void, only exit
 */
void cmdExit(char* argv[], int argc){
    if (argc != 2){ // Prevent segFault
        printf("Example exit command: 'exit 1'\n");
        return;
    }

    int code = atoi(argv[1]); // Get the exit code
    code = code & 0xFF;
    printf("bye\n");
    exit(code);
}

// FOR EXTRA CREDIT
/**
 * @name runPiped
 * @brief Imitating pipe in shell
 * @param argv arguments that run from the user input
 * @param argc number of arguments
 * @returns void, execute commands
 */
void runPiped(char* argv[], int argc){
    char** commands[10]; // Limit at 10 commands
    int idxCmds = 0;
    int idxArgs = 0;

    // My idea is that I want to make an array of array of arguments, so that it can be easily to deal with.
    commands[idxCmds] = malloc(sizeof(char*) * 64);

    for (int i = 0; i < argc && idxCmds < 10; i++) { // Appending arguments into commands until we find "|", then we go to the next iteration.
        if (strcmp(argv[i], "|") == 0) {
            commands[idxCmds++][idxArgs] = NULL; // End of array
            idxArgs = 0; // Reset index

            commands[idxCmds] = malloc(sizeof(char*) * 64);
            continue;
        }
        commands[idxCmds][idxArgs++] = strdup(argv[i]); // I just learn this strdup (string duplicate)
    }

    commands[idxCmds++][idxArgs] = NULL;

    // We have "idxCmds - 1" pipes, so we need twice of that for file discriptors
    int pipefd[2 * (idxCmds - 1)];

    for (int i = 0; i < idxCmds - 1; i++) { // Appending the file discriptors
        pipe(&pipefd[i * 2]); 
    }

    /*
    Fork new child for every command and redirect input/output of each child correctly, 
    then close all the file discriptors after finish. Execute the command at last.
    */ 
    for (int i = 0; i < idxCmds; i++) {
        pid_t pid;
        if ((pid = fork()) < 0){ // Error
            perror ("Fork failed");
            for (int j = 0; j < 2*(idxCmds - 1); j++) {
                close(pipefd[j]);
            }
            prevExitCode = 1;
            return;
        }
        if (!pid){ // Child
            if (i > 0){ // Redirect input for every child except the first one
                dup2(pipefd[(i-1)*2], 0); // 'Read' file discriptors are even indexes in pipefd
            }
            if (i != idxCmds-1) { // Redirect output for every child except the last one
                dup2(pipefd[(i*2)+1], 1); // 'Write' file discriptors are odd indexes in pipefd
            }
            for (int j = 0; j < 2*(idxCmds - 1); j++) { // Close all pipefd
                close(pipefd[j]);
            }

            execvp(commands[i][0], commands[i]); // Executing commands
            perror("execvp failed");
            exit(1);
        }
    }

    // Parent
    for (int i = 0; i < 2*(idxCmds - 1); i++) { // Close ll pipefd
        close(pipefd[i]);
    }

    for (int i = 0; i < idxCmds; i++) { // Waiting for every child to finish their processes
        wait(NULL);
    }
 }