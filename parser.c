#include "parser.h"

void bufferToArg(char* buffer, char** argv, char** input, char** output){
    int argc = 0;

    char* token = strtok(buffer, " ");
    while (token != NULL && argc < 63) {
        if (strcmp(token, "<") == 0){
            token = strtok(NULL, " ");
            *input = token;
        }
        else if (strcmp(token, ">") == 0){
            token = strtok(NULL, " ");
            *output = token;
        }
        else {
            argv[argc++] = token;
        }
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL; // Null-terminate
    return;
}
bool checkAmpersand(char** buffer){
    int len = strlen(*buffer);
    if ((*buffer)[len - 1] == '&'){
        (*buffer)[len - 2] = '\0';
        return true;
    }
    return false;
}
