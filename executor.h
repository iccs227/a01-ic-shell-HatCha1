#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "job.h"
#include "parser.h"

void runExternal(char* buffer, char* argv[], int argc, char* in, char* out);

#endif