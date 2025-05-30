#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_CMD_BUFFER 255
#define MAX_JOBS 30

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

extern char prevBuffer[255];
extern bool script;
extern int prevExitCode;

#endif