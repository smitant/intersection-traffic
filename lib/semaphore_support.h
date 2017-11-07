/*
 * Josh Hursey
 *
 * CS 441/541: Semaphore Wrapper Library
 *
 * On Mac we need to use 'named' semaphores because 'unnamed' semaphores
 * are not supported. On Linux and Cygwin we can use 'unnamed' semaphores.
 * Since these differ, programaticly, only by the way they are created and
 * destroyed then we can hide this complexity in a support library.
 *
 */
#ifndef SEM_SUPPORT
#define SEM_SUPPORT

#include <stdio.h>
#include <stdlib.h> /* For malloc */
#include <errno.h>
#include <unistd.h> /* For usleep */
#include <pthread.h>
#include <semaphore.h>

#if defined(__APPLE__)
//#error "This platform can support POSIX named semaphores, but not unnamed semaphores"
#define USE_NAMED_SEMAPHORES 0
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <unistd.h>          /* For getpid() */
#else
#define USE_NAMED_SEMAPHORES 1
#endif

/*
 * Semaphore type
 */
struct semaphore_t {
    sem_t *sem;
    char *name;
};
typedef struct semaphore_t semaphore_t;


/*
 * Create a POSIX Semaphore
 *
 * Parameters:
 *   sem : semaphore to create (pass-by-reference)
 *   value : initial value of the semaphore
 *
 * Returns:
 *   0 upon success,
 *   and non-0 if there was an error.
 */
int semaphore_create(semaphore_t *sem, unsigned int value);


/*
 * Destroy a POSIX Semaphore
 *
 * Parameters:
 *  sem : semaphore to destroy (pass-by-reference)
 *
 * Returns:
 *   0 upon success,
 *   and non-0 if there was an error.
 */
int semaphore_destroy(semaphore_t *sem);


/*
 * Wait on a POSIX Semaphore
 *
 * Parameters:
 *   sem : semaphore to wait on (pass-by-reference)
 *
 * Returns:
 *   0 upon success,
 *   and non-0 if there was an error.
 */
int semaphore_wait(semaphore_t *sem);


/*
 * Signal a POSIX Semaphore
 *
 * Parameters:
 *   sem : semaphore to signal (pass-by-reference)
 *
 * Returns:
 *   0 upon success,
 *   and non-0 if there was an error.
 */
int semaphore_post(semaphore_t *sem);


/*
 * Try to Wait on a POSIX Semaphore
 *
 * Parameters:
 *   sem : semaphore to wait on (pass-by-reference)
 *
 * Returns:
 *   0 if the lock was acquired.
 *  -1 if the lock was not acquired
 *   other non-0 if there was an error.
 */
int semaphore_trywait(semaphore_t *sem);

#endif /* SEM_SUPPORT */
