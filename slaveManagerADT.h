#ifndef __SLAVE_MANAGER_ADT_H__
#define __SLAVE_MANAGER_ADT_H__

enum pipe {READ=0, WRITE=1};

typedef struct slaveManagerCDT * slaveManagerADT;


slaveManagerADT newSlaveManager(char ** fileNames, int slavesQty, int fileCount);

int initializeSlaves(slaveManagerADT sm);

void waitSlaves(slaveManagerADT sm);

int hasNextData(slaveManagerADT sm);

#endif