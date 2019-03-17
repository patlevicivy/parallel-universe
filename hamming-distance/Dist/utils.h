#ifndef PAR1_MY_STRUCTS_H
#define PAR1_MY_STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <omp.h>
#include <stdint.h>

/**
 * Allocates memory for all arrays that are needed.
 * @param A The first matrix with strings of length l (size: m x l).
 * @param numOfStringsA Number of strings in the first matrix.
 * @param B The second matrix with strings of length l (size: m x l).
 * @param numOfStringsB Number of strings in the second matrix.
 * @param distances The matrix that stores the hamming distances (size: m x l).
 * @param len The length of each string.
 * @param sums Array that stores the sums calculated by each thread (size: {@see @param threads}).
 * @param threads Number of threads to use.
 */
void init_arrays(uint8_t ***A, int numOfStringsA, uint8_t ***B, int numOfStringsB, uint8_t ***distances, int len,
                 long long unsigned **sums, int threads);

/**
 * Frees up all memory used by arrays.
 * @param A The first matrix with strings of length l (size: m x l).
 * @param numOfStringsA Number of strings in the first matrix.
 * @param B The second matrix with strings of length l (size: m x l).
 * @param numOfStringsB Number of strings in the second matrix.
 * @param distances The matrix that stores the hamming distances (size: m x l).
 * @param len The length of each string.
 * @param sums Array that stores the sums calculated by each thread (size: {@see @param threads}).
 */
void free_arrays(uint8_t ***A, int numOfStringsA, uint8_t ***B, int numOfStringsB, uint8_t ***distances, int len, long long unsigned **sums);

/**
 * Calculates the current time.
 * @return The time of the day in us.
 */
double gettime(void);


/**
 * calculates the final sum of all local sums (calculated by each thread).
 * @param thread_sums Array that
 * @param totals
 * @return The sum of hamming distances from matrix distances.
 */
long long unsigned eval_threads_sum(long long unsigned *thread_sums, int totals);

/**
 * pthread argument struct, used in pthread_create().
 */
typedef struct thread_args {
	int numOfStringsA, numOfStringsB, len, tid, total_threads;
    uint8_t **A, **B, **distances;
    long long unsigned *sums;   //Each thread increments a cell of this array (size: threads).
} t_args;

/**
 *
 * @param A The first matrix with strings of length l (size: m x l).
 * @param B The second matrix with strings of length l (size: m x l).
 * @param numOfStringsA Number of strings in the first matrix.
 * @param numOfStringsB Number of strings in the second matrix.
 * @param distances The matrix that stores the hamming distances (size: m x l).
 * @param len The length of each string.
 * @param sums Array that stores the sums calculated by each thread.
 * @return A pointer to a t_args struct.
 */
t_args *
construct_targ(uint8_t **A, uint8_t **B, uint8_t **distances, int numOfStringsA, int numOfStringsB, int len, long long unsigned *sums, int tid,
               int threads);


/**
 * Calculates the sum without any form of parallelism.
 * @param A The 1st array.
 * @param numOfStringsA The size of the 1st array is m * l.
 * @param B The 2nd array.
 * @param numOfStringsB The size of the 2nd array is n * l.
 * @param distances The table that will hold the results.
 * @param len The string length.
 */
void compute_serial(uint8_t **A, int numOfStringsA, uint8_t **B, int numOfStringsB, uint8_t **distances, int len);


/**
 * Parallelize each task using the pthreads library.
 * @param A The 1st array.
 * @param numOfStringsA The size of the 1st array is m * l.
 * @param B The 2nd array.
 * @param numOfStringsB The size of the 2nd array is n * l.
 * @param distances The table that will hold the results.
 * @param len The string length.
 */
void compute_pthreads(uint8_t **A, int numOfStringsA, uint8_t **B, int numOfStringsB, uint8_t **distances, int len,
                      long long unsigned *sums, int threads);


//TASK
/**
 * Parallelize a string from the first table with every string from the second table.
 */
void *task(void *arguments);

#endif
