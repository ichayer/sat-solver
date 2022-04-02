#ifndef __SLAVE_MANAGER_ADT_H__
#define __SLAVE_MANAGER_ADT_H__


typedef struct slaveManagerCDT * slaveManagerADT;

slaveManagerADT newSlaveManager(char ** fileNames, int slavesQty, int fileCount);

int initializeSlaves(slaveManagerADT sm);



void waitSlaves(slaveManagerADT sm);

#endif