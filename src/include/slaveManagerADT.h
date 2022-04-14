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

/* Local headers */
#include "../include/lib.h"

enum pipe {READ=0, WRITE=1};

typedef struct slaveManagerCDT * slaveManagerADT;


/** 
 * @brief Creates a manager for de slaves that will process de files recibed
 *    
 * @param fileNames files that will be processed
 * @param fileCount quantity of files to be processed
 * 
 * @return a handler for the slaves and files processing
 */
slaveManagerADT newSlaveManager(char ** fileNames, int fileCount);

/** 
 * @brief Initializes the slaves in de Slave Manager received as param, sending to files of each of them
 *    
 * @param sm the slave manager whose slaves will be initialized
 * 
 * @return the numbre of slaves initialized
 */
int initializeSlaves(slaveManagerADT sm);


/** 
 * @brief Allows to know if the slave manager has pending data to be analized
 *    
 * @param sm the slave manager mentioned in brief
 * 
 * @return 0 (false) in case there are no pending files to be analized, 1 otherwise
 */
int hasNextData(slaveManagerADT sm);

/** 
 * @brief Returns the data analized of one file each time it is called in the buffer recibed as param.
 * If the bufferLimit is smaller than the amount of data that needs to be stored, the unsaved data will
 * be lost, since the next call to retriveData will return the data of the next file analized.
 * This function also is in charge of delivering a new file to the slave that analize the data in case
 * this slave does not have any more tasks to be done. 
 *    
 * @param sm the data is retrieved from this slave manager
 * @param buffer pointer to the memory where the data will be stored
 * @param bufferLimit space available in the buffer
 * 
 * @return 0 (false) in case there are no pending files to be analized, 1 otherwise
 */
int retriveData(slaveManagerADT sm, char * buffer, int bufferLimit);


/** 
 * @brief Frees the memory that the Slave Manager uses.
 */
void freeSlaveManager(slaveManagerADT sm);

#endif