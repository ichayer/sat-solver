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

    sleep(VIEW_PROCES_WAITTIME);

    //slaveManagerADT sm = newSlaveManager(argv, 2, 2);

    char * output[2] = {"test/test2.txt", "test/testFile.txt"};
    slaveManagerADT sm = newSlaveManager(output, 2, 2);

    initializeSlaves(sm);

    /*while(hasNextData(sm)){

    } */   

    waitSlaves(sm);


    return 0;
}