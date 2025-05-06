/* ICCS227: Project 1: icsh
 * Name: Pasu Kositanont
 * StudentID: 6680060
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"   
#include "signal.h"
#include <fcntl.h>

#define MAX_CMD_BUFFER 255

char* firstWord(char* buffer);
void allCommands(char* buffer);
void cmdEcho(char* buffer);
void cmdBang();
void cmdExit(char* buffer);
void runExternal(char* buffer);
void ignore(int signum);
void bufferToArg(char* buffer, char** argv, char** input, char** output);

char prevBuffer[255];
bool script = false;
int prevExitCode = 0;

int main(int argc, char* argv[]) {

    char buffer[MAX_CMD_BUFFER];

    signal(SIGTSTP, ignore);
    signal(SIGINT, ignore);

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
    else{
        runExternal(buffer);
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

void runExternal(char* buffer) {

    char* argv[64];

    char* in = NULL;
    char* out = NULL;
    bufferToArg(buffer, argv, &in, &out);

    pid_t pid;
    if ((pid=fork()) < 0){ //Error
        perror ("Fork failed");
        prevExitCode = 1;
        return;
    }
    if (!pid) { //Child
        if (in != NULL){
            int fd = open(in, O_RDONLY);
            if (fd < 0){
                perror("No Such File");
            }
            dup2(fd, 0);
        }
        if (out != NULL){
            int fd = open(out, O_TRUNC | O_CREAT | O_WRONLY, 0666);
            dup2(fd, 1);
        }
        execvp(argv[0], argv);
        perror("");
        exit(1);
    }
    else { //Parent

        int stat;
        waitpid(pid, &stat, WUNTRACED);

        if (WIFSTOPPED(stat)) { //https://linux.die.net/man/3/waitpid
            printf("\n[%d] Stopped\t%s\n", pid, argv[0]);
        }
        if (WIFEXITED(stat)) {
            prevExitCode = WEXITSTATUS(stat);
        } else {
            prevExitCode = 1;
        }
    }
}

void ignore(int signum){
    printf("\n");
    fflush(stdout);
}

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
