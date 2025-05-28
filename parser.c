#include "parser.h"

/**
 * @name bufferToArg
 * @brief Convert bufffer to arguments
 * @param buffer that you want to convert
 * @param argv that you want to convert to
 * @param input Check if < is shown in the buffer
 * @param output Check if > is shown in the buffer
 * @returns int, argc
 */
int bufferToArg(char* buffer, char** argv, char** input, char** output){
    int argc = 0;
    char* token = strtok(buffer, " ");

    while (token != NULL && argc < 63) { // Using strtok to get each arguments untill the end of buffer
        if (strcmp(token, "<") == 0){  // If < is found, argv is everything before < and input is after <
            token = strtok(NULL, " ");
            *input = token;
        }
        else if (strcmp(token, ">") == 0){ // If < is found, argv is everything before > and output is after >
            token = strtok(NULL, " ");
            *output = token;
            if (token == NULL) {
                printf("No file specified for output redirection");
            }
        }
        else {
            argv[argc++] = token;
        }
        token = strtok(NULL, " ");
        
    }
    argv[argc] = NULL; // Null-terminate
    return argc;
}

/**
 * @name checkAmpersand
 * @brief Check if & is in the arguments, then remove it
 * @param argv arguments that you want to check
 * @param argc number of arguments
 * @returns bool, found or not found
 */
bool checkAmpersand(char** argv, int argc){
    if (strcmp(argv[argc-1], "&") == 0) {
        argv[argc-1] = NULL;
        return true;
    }
    return false;
}