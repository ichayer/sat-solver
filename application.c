#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "slaveManagerADT.h"

#define VIEW_PROCES_WAITTIME 2

int
main(int argc, char *argv[]){


    setvbuf(stdout, NULL, _IONBF, 0);

    //sleep(VIEW_PROCES_WAITTIME);
    
    //slaveManagerADT sm = newSlaveManager(argv, 2, 2);

    char * output[] = {"test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba","test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba","test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba"};
    slaveManagerADT sm = newSlaveManager(output, 20);

    initializeSlaves(sm);
    
    char buffer[4096];
    int read;

    while(hasNextData(sm)){
        read = retriveData(sm, buffer, 4096);
        write(STDIN_FILENO, buffer, read);
        write(STDIN_FILENO, "\n", 1);
    }

    freeSlaveManager(sm);  

    return 0;
}