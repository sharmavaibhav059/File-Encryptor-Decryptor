/* fsd.h */ 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <termios.h>
#include "arcfour.h"
#include <fcntl.h>

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

bool echo(bool);
int8 *readkey(int8*);
void usage(int8*);
int16 grabmgaic( int32);
int main(int, char**);
