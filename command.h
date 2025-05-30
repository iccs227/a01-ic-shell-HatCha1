#ifndef COMMAND_H
#define COMMAND_H

#include "job.h"
#include "built_in.h"
#include "executor.h"
#include "parser.h"

char* firstWord(char* buffer);
void allCommands(char* buffer);

#endif