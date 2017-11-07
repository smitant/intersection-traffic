/*
 * Josh Hursey
 *
 * CS 441/541: Pthreads Example
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "semaphore_support.h"

/*****************************
 * Defines
 *****************************/
/* Number of threads to create */
#define NUM_THREADS 10

/* Max Buffer size */
#define BUFFER_SIZE 250


/*****************************
 * Function Declarations
 *****************************/
/*
 * Thread Start Function
 * Add a segment of numbers in the buffer
 * Store the result in the segment of 'subtotals' for this thread.
 *
 * Parameters:
 *  *threadid : integer artificial thread ID
 *
 * Returns:
 *  NULL
 */
void *add_numbers(void *threadid);
