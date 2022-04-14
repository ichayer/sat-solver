#ifndef __LIB_H__
#define __LIB_H__

/* Standard library */
#include <sys/select.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

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

/**
 * @brief Maps one file descriptor to another.  
 * 
 * @param fd vector of fd
 * @param idxFrom index of the file descriptor that will be remaped
 * @param fdTo fd that the fd[idxFrom] fd will be mapped to
 */
void remapfd(int * fd, int idxFrom, int fdTo);

/**
 * @brief returns the max value of a vector
 */
int maxValue(int * vec, int qty);

/**
 * @brief Set the Fds in vec in rfds
 */
void setFds(int * vec, int qty, fd_set * rfds);

#endif