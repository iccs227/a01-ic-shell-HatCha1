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