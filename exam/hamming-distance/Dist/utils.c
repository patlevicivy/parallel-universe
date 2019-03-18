#include "utils.h"


double gettime(void) {
	clock_t begin = clock();
	return (begin);
}

void init_arrays(uint8_t ***A, int numOfStringsA, uint8_t ***B, int numOfStringsB, uint8_t ***distances, int len,
                 long long unsigned **sums, int threads) {
	*A = (uint8_t **)malloc(numOfStringsA * sizeof(uint8_t *));
	for (int i = 0; i < numOfStringsA; i++) {
        (*A)[i] = (uint8_t *) malloc(len * sizeof(uint8_t));
        for (int j = 0; j < len; j++) {
            (*A)[i][j] = rand() % 2;
        }
    }

	*B = (uint8_t **)malloc(numOfStringsB * sizeof(uint8_t *));
	for (int i = 0; i < numOfStringsB; i++) {
        (*B)[i] = (uint8_t *) malloc(len * sizeof(uint8_t));
        for (int j = 0; j < len; j++) {
            (*B)[i][j] = rand() % 2;
        }
    }

	(*distances) = (uint8_t **)malloc(numOfStringsA * sizeof(uint8_t *));
	for (int i = 0; i < numOfStringsA; i++) {
		(*distances)[i] = (uint8_t *)malloc(numOfStringsB * sizeof(uint8_t));
		for (int j = 0; j < numOfStringsB; j++) {
            (*distances)[i][j] = 0;
        }
    }

    (*sums) = malloc(threads * sizeof(long long unsigned));
}

void free_arrays(uint8_t ***A, int numOfStringsA, uint8_t ***B, int numOfStringsB, uint8_t ***distances, int len, long long unsigned **sums) {
	for (int i = 0; i < numOfStringsA; i++)
        free((*A)[i]);
    free(*A);

	for (int i = 0; i < numOfStringsB; i++)
        free((*B)[i]);
    free(*B);

	for (int i = 0; i < numOfStringsA; i++)
        free((*distances)[i]);
    free(*distances);

    free(*sums);
}

long long unsigned eval_threads_sum(long long unsigned *thread_sums, int total) {
    long long unsigned thread_sum = 0;
    for (int i = 0; i < total; i++) {
        thread_sum += thread_sums[i];
    }
    return thread_sum;
}

t_args *
construct_targ(uint8_t **A, uint8_t **B, uint8_t **distances, int numOfStringsA, int numOfStringsB, int len, long long unsigned *sums, int tid, int threads) {
    t_args *targ = (t_args *) malloc(sizeof(t_args));
	targ->numOfStringsA = numOfStringsA;
	targ->numOfStringsB = numOfStringsA;
    targ->len = len;
    targ->A = A;
    targ->B = B;
    targ->distances = distances;
    targ->sums = sums;
    targ->tid = tid;
    targ->total_threads = threads;
    return targ;
}