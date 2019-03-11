#include "utils.h"


double gettime(void) {
	clock_t begin = clock();
    //struct timeval ttime;
    //gettimeofday(&ttime, NULL);
	return begin;//ttime.tv_sec + ttime.tv_usec * 0.000001;
}

void init_arrays(uint8_t ***A, int m, uint8_t ***B, int n, uint8_t ***distances, int l,
                 long long unsigned **sums, int threads) {
    *A = (uint8_t **) malloc(m * sizeof(uint8_t * ));
    for (int i = 0; i < m; i++) {
        (*A)[i] = (uint8_t *) malloc(l * sizeof(uint8_t));
        for (int j = 0; j < l; j++) {
            (*A)[i][j] = rand() % 2;
        }
    }

    *B = (uint8_t **) malloc(n * sizeof(uint8_t * ));
    for (int i = 0; i < n; i++) {
        (*B)[i] = (uint8_t *) malloc(l * sizeof(uint8_t));
        for (int j = 0; j < l; j++) {
            (*B)[i][j] = rand() % 2;
        }
    }

    (*distances) = (uint8_t **) malloc(m * sizeof(uint8_t * ));
    for (int i = 0; i < m; i++) {
        (*distances)[i] = (uint8_t *) malloc(n * sizeof(uint8_t));
        for (int j = 0; j < n; j++) {
            (*distances)[i][j] = 0;
        }
    }

    (*sums) = malloc(threads * sizeof(long long unsigned));
}

void free_arrays(uint8_t ***A, int m, uint8_t ***B, int n, uint8_t ***distances, int l, long long unsigned **sums) {
    for (int i = 0; i < m; i++)
        free((*A)[i]);
    free(*A);

    for (int i = 0; i < n; i++)
        free((*B)[i]);
    free(*B);

    for (int i = 0; i < m; i++)
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
construct_targ(uint8_t **A, uint8_t **B, uint8_t **distances, int m, int n, int l, long long unsigned *sums, int tid, int threads) {
    t_args *targ = (t_args *) malloc(sizeof(t_args));
    targ->m = m;
    targ->n = n;
    targ->l = l;
    targ->A = A;
    targ->B = B;
    targ->distances = distances;
    targ->sums = sums;
    targ->tid = tid;
    targ->total_threads = threads;
    return targ;
}