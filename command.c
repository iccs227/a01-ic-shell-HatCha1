#include "command.h"

char* firstWord(char* buffer){ //https://stackoverflow.com/questions/42605304/how-do-i-split-a-string-at-the-first-space-and-consign-the-strings-to-different
    char* startOfSecond = strchr(buffer, ' '); // " world";
    size_t lengthOfFirst;

    if (startOfSecond != NULL) {
        lengthOfFirst = startOfSecond - buffer;
    } else {
        lengthOfFirst = strlen(buffer);
    }

    char* command = (char*)malloc((lengthOfFirst + 1) * sizeof(char));
    strncpy(command, buffer, lengthOfFirst);
    command[lengthOfFirst] = '\0';
    return command;
}

void allCommands(char* buffer){

    char* command = firstWord(buffer); //Getting the command line

    if(strlen(command) == 0){ //If type nothing
        return;
    }
    if (strcmp(buffer, "echo $?") == 0){
        printf("Exit Code: %d\n", prevExitCode);
    }
    else if(strcmp(command, "echo") == 0){ //Cmd: echo ...
        strcpy(prevBuffer, buffer);
        cmdEcho(buffer);
        prevExitCode = 0;
    }

    else if(strcmp(command, "!!") == 0){ //Cmd: !!
        cmdBang();
        prevExitCode = 0;
    }

    else if(strcmp(command, "exit") == 0){ //Cmd: exit ..
        free(command);
        cmdExit(buffer);
        prevExitCode = 0;
    }
    else if(strcmp(command, "##") == 0 && script){
        return;
        prevExitCode = 0;
    }
    else if (strcmp(command, "jobs") == 0 ){
        cmdJobs();
        prevExitCode = 0;
    }
    else if (strcmp(command, "fg") == 0){
        cmdFg(buffer);
        prevExitCode = 0;
    }
    else if (strcmp(command, "bg") == 0){
        cmdBg(buffer);
        prevExitCode = 0;
    }
    else{
        runExternal(buffer);
    }

    free(command);

}