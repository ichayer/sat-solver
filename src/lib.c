// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Standard library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

/* Local headers */
#include "./include/lib.h"

void processDetail() {

    system("cat /proc/1/status");
}

void listOpenFiles(){

    char lsof[50]= "lsof -p";
    char pid = (long) getpid() + '0';
    strcat(lsof, &pid);
    system(lsof);
}

void perrorExit(char * message){
    perror(message);
    exit(EXIT_FAILURE);
}