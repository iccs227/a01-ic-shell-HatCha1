#ifndef BUILT_IN_H
#define BUILT_IN_H

#include "globals.h"
#include "command.h"
#include "parser.h"

void cmdEcho(char* argv[]);
void cmdBang(char* buffer, char* argv[]);
void cmdExit(char* argv[], int argc);
void runPiped(char* argv[], int argc);

#endif