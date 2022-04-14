#ifndef __LIB_H__
#define __LIB_H__

/* Constants */
#define SLAVES_MAX_OUTPUT 4096

#define OUTPUT_FILE_NAME "resultado.txt"
#define WR_ALL 00666

/** 
 * @brief Prints a system error message and exit with EXIT_FAILURE
 *    
 * @param message message to print
 */
void perrorExit(char * message);

#endif