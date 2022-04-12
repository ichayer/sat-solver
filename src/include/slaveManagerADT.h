#ifndef __SLAVE_MANAGER_ADT_H__
#define __SLAVE_MANAGER_ADT_H__

/* Standard library */
#include <sys/select.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAX_OUTPUT_LENGTH 4096

enum pipe {READ=0, WRITE=1};

typedef struct slaveManagerCDT * slaveManagerADT;


slaveManagerADT newSlaveManager(char ** fileNames, int fileCount);

int initializeSlaves(slaveManagerADT sm);

int hasNextData(slaveManagerADT sm);

int retriveData(slaveManagerADT sm, char * buffer, int bufferLimit);

void freeSlaveManager(slaveManagerADT sm);

#endif