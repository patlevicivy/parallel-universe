#include "pthread.h"
#include "utils.h"

void compute_pthreads(uint8_t **A, int numOfStringsA, uint8_t **B, int numOfStringsB, uint8_t **distances,
                      int len, long long unsigned *sums, int threads) {

    pthread_t *TT = (pthread_t *) malloc(threads * sizeof(pthread_t));
    t_args **argTable = (t_args **) malloc(threads * sizeof(t_args *));

    for (int i = 0; i < threads; i++) {
		argTable[i] = construct_targ(A, B, distances, numOfStringsA, numOfStringsB, len, sums, -1, threads);
    }

    double start_time = gettime();

    for (int t = 0; t < threads; t++) {
        argTable[t]->tid = t;
        
                if (pthread_create(&TT[t], NULL, task, (void *) argTable[t])) {
                    printf("Error creating thread C with tid=%d\n", t);
                    assert(0);
                }
    }

    for (int t = 0; t < threads; t++) {
        pthread_join(TT[t], NULL);
        free(argTable[t]);
    }

    free(TT);
    long long unsigned sum = eval_threads_sum(sums, threads);

	printf("threads:%d\ttime:%lf\tsum:%llu\n", threads, (gettime() - start_time)/CLOCKS_PER_SEC,sum);
}

void *task(void *arguments) {
    t_args *args = arguments;
    uint8_t **A = args->A;
    uint8_t **B = args->B;
    uint8_t **distances = args->distances;
	int numOfStringsA = args->numOfStringsA;
	int numOfStringsB = args->numOfStringsB;
    int len = args->len;
    int threads = args->total_threads;
    int tid = args->tid;
    unsigned long long int *sums = args->sums;
	long long unsigned sum = 0;

	for (int i = tid; i < numOfStringsA; i += threads) {
		for (int j = 0; j < numOfStringsB; j++) {
            for (int k = 0; k < len; k++) {
                if (A[i][k] != B[j][k]) {
                    distances[i][j]++;
					sum++;
                }
            }
        }
    }
	sums[tid] = sum;
    pthread_exit(NULL);
}
