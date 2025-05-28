#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int bufferToArg(char* buffer, char** argv, char** input, char** output);
bool checkAmpersand(char** argv, int argc);

#endif