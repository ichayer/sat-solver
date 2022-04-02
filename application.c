#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "slaveManagerADT.h"


int
main(int argc, char *argv[]){

    char * output[10];

    slaveManagerADT sm = newSlaveManager(argv, 2, 2);

    initializeSlaves(sm);

    


    waitSlaves(sm);

    return 0;
}