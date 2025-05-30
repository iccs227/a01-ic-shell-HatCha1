/* ICCS227: Project 1: icsh
 * Name: Pasu Kositanont
 * StudentID: 6680060
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_CMD_BUFFER 255

char* firstWord(char* buffer);
int bufferToArg(char* buffer, char** argv);
void allCommands(char* buffer);
void cmdEcho(char* argv[]);
void cmdBang();
void cmdExit(char* argv[], int argc);

char prevBuffer[255];
bool script = false;

int main(int argc, char* argv[]) {
    char buffer[MAX_CMD_BUFFER];
    
    if (argc > 1){
        script = true;
        FILE* file = fopen(argv[1], "r");

        if (file == NULL){
            printf("No such file %s\n", argv[1]);
            return 0;
        }

        while (fgets(buffer, sizeof(buffer), file)) { // Read file line by line and run the command until the end

            buffer[strcspn(buffer, "\n")] = '\0';
            
            if (buffer[0] != '!' && buffer[1] != '!'){
                strncpy(prevBuffer, buffer, MAX_CMD_BUFFER);
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
        
        if (buffer[0] != '!' && buffer[1] != '!'){
            strncpy(prevBuffer, buffer, MAX_CMD_BUFFER);
        }

        allCommands(buffer);
    }
}

/**
 * @name bufferToArg
 * @brief Convert bufffer to arguments
 * @param buffer that you want to convert
 * @param argv that you want to convert to
 * @returns int, argc
 */
int bufferToArg(char* buffer, char** argv){
    int argc = 0;
    char* token = strtok(buffer, " ");

    while (token != NULL && argc < 63) { // Using strtok to get each arguments untill the end of buffer
        argv[argc++] = token;
        token = strtok(NULL, " ");
        
    }
    argv[argc] = NULL; // Null-terminate
    return argc;
}

/**
 * @name allCommands
 * @brief Function to run necessary commands in this shell 
 * @param buffer The orginal user input
 * @returns void, only run other commands
 */
void allCommands(char* buffer){

    char* argv[64];
    char copyBuffer[MAX_CMD_BUFFER];

    strncpy(copyBuffer, buffer, MAX_CMD_BUFFER);
    int argc = bufferToArg(copyBuffer, argv);

    char* command = argv[0];

    if(command == NULL){ //If type nothing
        return;
    }

    if(strcmp(command, "echo") == 0){ //Cmd: echo ...
        cmdEcho(argv);
    }

    else if(strcmp(command, "!!") == 0){ //Cmd: !!
        cmdBang();
    }
    else if(strcmp(command, "exit") == 0){ //Cmd: exit ..
        cmdExit(argv, argc);
    }
    else if(strcmp(command, "##") == 0 && script){ // In script mode, if ## is shown, we do not want to include them as a command line
        return;
    }
    else{
        printf("bad command\n"); //Cmd: bad command
    }
}

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
void cmdBang(){
    if (strlen(prevBuffer) == 0){
        return;
    }
    printf("%s\n", prevBuffer);
    allCommands(prevBuffer);
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