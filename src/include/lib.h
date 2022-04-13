#ifndef __LIB_H__
#define __LIB_H__

/* Constants */
#define SLAVES_MAX_OUTPUT 4096

/**
 * @brief Prompts process information. 
 * 
 */
void processDetail();

/**
 * @brief list open files of the current process
 * 
 */
void listOpenFiles();

/** 
 * @brief Prints a system error message and exit with EXIT_FAILURE
 *    
 * @param message message to print
 */
void perrorExit(char * message);

#endif