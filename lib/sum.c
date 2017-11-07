/*
 * Josh Hursey
 *
 * CS 441/541: Example - add some numbers
 *
 */
#include "sum.h"

/*
 * Buffer of random integers
 * Shared between all threads in the process
 */
int buffer[BUFFER_SIZE];

/*
 * A place to accumulate the total
 */
long total = 0;


/*
 * Semaphore(s)
 */
semaphore_t total_lock;


int main(int argc, char * argv[]) {
    int ret, i;
    pthread_t threads[NUM_THREADS];

    /*
     * Initialize:
     * - buffer
     * - random number generator
     * - semaphores
     */
    srandom(time(NULL));
    for(i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = random() % 1024;
    }

    if( 0 != (ret = semaphore_create(&total_lock, 1)) ) {
        fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
        return -1;
    }

    /*
     * Create thread(s)
     */
    for(i = 0; i < NUM_THREADS; ++i) {
        printf("In Main(): Creating thread %d\n", i);
        ret = pthread_create(&threads[i],
                             NULL,
                             add_numbers,
                             (void *)(intptr_t)i);
        if(0 != ret ) {
            fprintf(stderr, "Error: Cannot Create thread\n");
            exit(-1);
        }
    }

    /*
     * Join Thread(s)
     */
    for(i = 0; i < NUM_THREADS; ++i ) {
        ret = pthread_join(threads[i], NULL);
        if( 0 != ret ) {
            fprintf(stderr, "Error: Cannot Join Thread %d\n", i);
            exit(-1);
        }
    }

    printf("The sum of %5d random numbers is    : %ld\n",
           BUFFER_SIZE, total);
    printf("The average of %5d random numbers is: %6.2f\n",
           BUFFER_SIZE, (total/(double)BUFFER_SIZE));

    /*
     * Cleanup
     */
    if( 0 != (ret = semaphore_destroy(&total_lock)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }

    pthread_exit(NULL);

    return 0;
}

void *add_numbers(void *threadid) {
    int tid = (intptr_t)threadid;
    int start_idx, end_idx;
    int idx, ret;
    long subtotal = 0;

    /*
     * Determine my indexes
     */
    start_idx =  tid      * (BUFFER_SIZE / NUM_THREADS);
    if( tid == (NUM_THREADS - 1) ) {
        end_idx = BUFFER_SIZE - 1;
    } else {
        end_idx   = (tid + 1) * (BUFFER_SIZE / NUM_THREADS) -1;
    }


    /*
     * Create my subtotal
     */
    printf("Thread %d: Start %d End %d\n",
           tid, start_idx, end_idx);
    for(idx = start_idx; idx <= end_idx; ++idx) {
        subtotal += buffer[idx];
    }
    printf("Thread %d: Start %d End %d = %ld\n",
           tid, start_idx, end_idx, subtotal );


    /*
     * Critical Section to update total
     */
    /* Enter the critical section */
    if( 0 != (ret = semaphore_wait(&total_lock)) ) {
        fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
        pthread_exit(NULL);
    }

    printf("Thread %2d: Inside Critical Section      (%8ld)\n", tid, total);
    total += subtotal;

    sleep(2); /* Just to simulate some work, so we can see the threads synchronize */
    printf("Thread %2d: Leaving the Critical Section (%8ld)\n", tid, total);

    /* Leave the critical section */
    if( 0 != (ret = semaphore_post(&total_lock)) ) {
        fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
        pthread_exit(NULL);
    }

    /*
     * All done!
     */
    pthread_exit(NULL);
}
