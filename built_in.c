#include "built_in.h"

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