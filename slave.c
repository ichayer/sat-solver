#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "myerror.h"

#define INITIAL_OUTPUT_LENGTH 4096
#define MAX_COMMAND_LENGTH 4096
#define MINISAT_COMMAND "minisat %s | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\""

/*
** Cosas asumidas: 
**  - El comando a ejecutar no tendra mas de 4096 caracteres.
**
** Problemas: Hubo que agregar el define de gnu para que funcione bien el getline.
** 
** Fuentes: 
** - Para sacar el \n de los paths: https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input 
**/

void formatOutput(char ** minisatOutput, int initialDim, FILE * commandStream);

int
main(int argc, char *argv[]){

    // read from stdin
    char * fileName = NULL;
    size_t lineCap = 0;
    ssize_t lineLen;  
   
    while ((lineLen = getline(&fileName, &lineCap, stdin)) > 0){

        fileName[strcspn(fileName, "\n")] = 0;  // https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input

        char command[MAX_COMMAND_LENGTH];

        if (sprintf(command, MINISAT_COMMAND, fileName) < 0){
            perrorexit("sprintf");
        }
        
        FILE * commandStream = popen(command, "r");
        if(commandStream==NULL)
            perrorexit("popen");
        
        char * minisatOutput = NULL;
        formatOutput(&minisatOutput, 0, commandStream);      

        printf("File name: %s, slave ID: %d, %s \n", fileName, getpid(), minisatOutput);
        free(minisatOutput);        
    }
    
    free(fileName);
    return 0;
}

void formatOutput(char ** minisatOutput, int dim, FILE * commandStream){

    int i = 0;
    int c;

    if(*minisatOutput == NULL){
        *minisatOutput = malloc((sizeof(char)) * INITIAL_OUTPUT_LENGTH);
        dim = INITIAL_OUTPUT_LENGTH;
    }
        
    char * out = *minisatOutput;

    while((c=getc(commandStream))>0){

        if(i == dim){
            dim+= INITIAL_OUTPUT_LENGTH;
            out = realloc(out, dim);
        }

        if(c=='\n'){
            out[i++] = ',';
            out[i++] = ' ';
        }
        else if(i==0 || c!=' ' || out[i-1]!=' ')
            out[i++] = c;
    }
    out[i-2] = 0;

    *minisatOutput = out;
}
