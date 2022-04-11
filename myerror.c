// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdio.h>
#include <stdlib.h>
#include "myerror.h"

void perrorexit(char * fname)
{
    perror(fname);
    printf("lla");
    exit(EXIT_FAILURE);
}