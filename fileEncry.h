/* fse.h */ 

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "arcfour.h"
#include <assert.h> 
#include <sys/random.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>

void changeecho(bool);
int8 *securerand(int16);
int8 *readkey(char*);
int main(int, char**);
