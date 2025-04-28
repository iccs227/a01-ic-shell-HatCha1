/* ICCS227: Project 1: icsh
 * Name: Pasu Kositanont
 * StudentID: 6680060
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

#define MAX_CMD_BUFFER 255

char* firstWord(char* buffer);
void allCommands(char* buffer);
void cmdEcho(char* buffer);
void cmdBang();
void cmdExit(char* buffer);

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

    if(strcmp(command, "echo") == 0){ //Cmd: echo ...
        strcpy(prevBuffer, buffer);
        cmdEcho(buffer);
    }

    else if(strcmp(command, "!!") == 0){ //Cmd: !!
        cmdBang();
    }

    else if(strcmp(command, "exit") == 0){ //Cmd: exit ..
        free(command);
        cmdExit(buffer);
    }
    else if(strcmp(command, "##") == 0 && script){ //Cmd: exit ..
        return;
    }
    else{
        printf("bad command\n"); //Cmd: bad command
    }

    free(command);

}

void cmdEcho(char* buffer){
    char* startOfSecond = strchr(buffer, ' ');

    if (startOfSecond[0] == ' ') //https://stackoverflow.com/questions/4295754/how-to-remove-first-character-from-c-string
        memmove(startOfSecond, startOfSecond+1, strlen(startOfSecond));

    printf("%s\n", startOfSecond);
}

void cmdBang(){
    if (strlen(prevBuffer) == 0){
        return;
    }
    if (!script){
        printf("%s\n", prevBuffer);
    }
    allCommands(prevBuffer);
}

void cmdExit(char* buffer){
    char* startOfSecond = strchr(buffer, ' ');

    if (startOfSecond[0] == ' ') //https://stackoverflow.com/questions/4295754/how-to-remove-first-character-from-c-string
        memmove(startOfSecond, startOfSecond+1, strlen(startOfSecond));

    int code = atoi(startOfSecond);
    code = code & 0xFF;
    printf("bye\n");
    exit(code);
}