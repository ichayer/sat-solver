// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Standard library */
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* Local headers */
#include "./include/lib.h"

/* Constants */
#define MAX_COMMAND_LENGTH 4096
#define MINISAT_COMMAND "minisat %s | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\""

void formatOutput(char * minisatOutput, FILE * commandStream, int maxDim);

int main(int argc, char *argv[]){

    // read from stdin
    char * fileName = NULL;
    size_t lineCap = 0;
    ssize_t lineLen;  

    setvbuf(stdout, NULL, _IONBF, 0);
   
    while ((lineLen = getline(&fileName, &lineCap, stdin)) > 0){

        fileName[strcspn(fileName, "\n")] = 0;  // https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input

        char command[MAX_COMMAND_LENGTH];

        if (sprintf(command, MINISAT_COMMAND, fileName) < 0){
            perrorExit("Error in sprintf function");
        }
        
        FILE * commandStream = popen(command, "r");
        if(commandStream==NULL)
            perrorExit("Error in popen function");
        
        char minisatOutput[SLAVES_MAX_OUTPUT];
        formatOutput(minisatOutput, commandStream, SLAVES_MAX_OUTPUT);      

        printf("< File name: %s, slave ID: %d, %s >", fileName, getpid(), minisatOutput);        
    }
    
    free(fileName);
    close(STDIN_FILENO);
    return 0;
}

void formatOutput(char * minisatOutput,  FILE * commandStream, int maxDim){
    int c;
    int i=0;

    while((c=getc(commandStream))>0 && i<maxDim){

        if(c=='\n'){
            minisatOutput[i++] = ',';
            minisatOutput[i++] = ' ';
        }
        else if(i==0 || c!=' ' || minisatOutput[i-1]!=' ')
            minisatOutput[i++] = c;
    }
    minisatOutput[i-2] = 0;
}
