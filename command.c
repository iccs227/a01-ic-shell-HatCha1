#include "command.h"
/**
 * @name allCommands
 * @brief Function to run necessary commands in this shell 
 * @param buffer The orginal user input
 * @returns void, only run other commands
 */
void allCommands(char* buffer){
    // Initialize variables for future uses
    char* argv[64];
    char copyBuffer[MAX_CMD_BUFFER];
    char* in = NULL;
    char* out = NULL;

    int saved_stdout = dup(1); // Save output direction
    int saved_stderr = dup(2); // Save error direction
    int fd = -1;

    strncpy(copyBuffer, buffer, MAX_CMD_BUFFER);
    int argc = bufferToArg(copyBuffer, argv, &in, &out); 

    // Now, argv, argc, in, and out are filled with values

    char* command = argv[0]; // Getting command

    if (out != NULL){ // If > is in the buffer, redirect output/error to this file
        fd = open(out, O_TRUNC | O_CREAT | O_WRONLY, 0666);
        dup2(fd, 1);
        dup2(fd, 2);
    }

    if(command == NULL){ //If type nothing
        return;
    }
    else if (strchr(buffer, '|') != NULL){ //Cmd: pipe
        runPiped(argv, argc);
        prevExitCode = 0;
    }
    else if (strcmp(buffer, "echo $?") == 0){ // For getting exit code
        printf("Exit Code: %d\n", prevExitCode);
    }
    else if(strcmp(command, "echo") == 0){ //Cmd: echo ...
        cmdEcho(argv);
        prevExitCode = 0;
    }

    else if(strcmp(command, "!!") == 0){ //Cmd: !!
        cmdBang(prevBuffer, argv);
        prevExitCode = 0;
    }

    else if(strcmp(command, "exit") == 0){ //Cmd: exit ..
        cmdExit(argv, argc);
        prevExitCode = 0;
    }
    else if(strcmp(command, "##") == 0 && script){ // In script mode, if ## is shown, we do not want to include them as a command line
        return;
        prevExitCode = 0;
    }
    else if (strcmp(command, "jobs") == 0 ){ //Cmd: jobs
        cmdJobs();
        prevExitCode = 0;
    }
    else if (strcmp(command, "fg") == 0){ //Cmd: fg %..
        cmdFg(argv[1], argc);
        prevExitCode = 0;
    }
    else if (strcmp(command, "bg") == 0){ //Cmd: bg %..
        cmdBg(argv[1], argc);
        prevExitCode = 0;
    }
    else{
        runExternal(buffer, argv, argc, in, out); // Run command externally
    }
    
    fflush(stdout); // flush output buffer
    fflush(stderr); // flush error

    dup2(saved_stdout, 1); // Get back output direction
    dup2(saved_stderr, 2); // Get back error direction
    if (fd >= 0) {
        close(fd); 
    }
    close(saved_stdout);
    close(saved_stderr);
}