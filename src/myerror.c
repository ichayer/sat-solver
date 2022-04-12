// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Standard library */
#include <stdio.h>
#include <stdlib.h>

/* Local headers */
#include "./include/myerror.h"

void perrorExit(char * message){
    perror(message);
    exit(EXIT_FAILURE);
}