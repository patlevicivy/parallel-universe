#include <pthread.h>
#include "utils.h"

void compute_pthreads(uint8_t **A, int m, uint8_t **B, int n, uint8_t **distances,
                      int l, int mode, long long unsigned *sums, int threads) {

    pthread_t *TT = (pthread_t *) malloc(threads * sizeof(pthread_t));
    t_args **argTable = (t_args **) malloc(threads * sizeof(t_args *));
    for (int i = 0; i < threads; i++) {
        argTable[i] = construct_targ(A, B, distances, m, n, l, sums, -1, threads);
    }

    double start_time = gettime();

    for (int t = 0; t < threads; t++) {
        argTable[t]->tid = t;
        switch (mode) {
            case 1:
                if (pthread_create(&TT[t], NULL, taskA, (void *) argTable[t])) {
                    printf("Error creating thread A with tid=%d\n", t);
                    assert(0);
                }
                break;
            case 2:
                if (pthread_create(&TT[t], NULL, taskB, (void *) argTable[t])) {
                    printf("Error creating thread B with tid=%d\n", t);
                    assert(0);
                }
                break;
            case 3:
                if (pthread_create(&TT[t], NULL, taskC, (void *) argTable[t])) {
                    printf("Error creating thread C with tid=%d\n", t);
                    assert(0);
                }
                break;
            default:
                printf("\nDefault case at pthreads.");
                break;
        }

    }


    for (int t = 0; t < threads; t++) {
        pthread_join(TT[t], NULL);
        free(argTable[t]);
    }

    free(TT);
    long long unsigned sum = eval_threads_sum(sums, threads);

    printf("threads:%d\tchunk:%d\tsum:%llu\ttime:%lf", threads, CHUNK, sum, gettime() - start_time);
}

/*TASKS*/

void *taskA(void *arguments) {
    t_args *args = arguments;
    uint8_t **A = args->A;
    uint8_t **B = args->B;
    uint8_t **distances = args->distances;
    int m = args->m;
    int n = args->n;
    int l = args->l;
    int threads = args->total_threads;
    int tid = args->tid;
    unsigned long long int *sums = args->sums;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = tid * CHUNK; k < l; k += threads * CHUNK) {
                int offset = 0;
                while (offset < CHUNK && k + offset < l) {
                    if (A[i][k + offset] != B[j][k + offset]) {
                        distances[i][j]++;
                        sums[tid]++;
                    }
                    offset++;
                }
            }
        }
    }
    pthread_exit(NULL);
}

void *taskB(void *arguments) {
    t_args *args = arguments;
    uint8_t **A = args->A;
    uint8_t **B = args->B;
    uint8_t **distances = args->distances;
    int m = args->m;
    int n = args->n;
    int l = args->l;
    int threads = args->total_threads;
    int tid = args->tid;
    unsigned long long int *sums = args->sums;

    for (int i = 0; i < m; i++) {
        for (int j = tid; j < n; j += threads) {
            for (int k = 0; k < l; k++) {
                if (A[i][k] != B[j][k]) {
                    distances[i][j]++;
                    sums[tid]++;
                }
            }
        }
    }
    pthread_exit(NULL);
}

void *taskC(void *arguments) {
    t_args *args = arguments;
    uint8_t **A = args->A;
    uint8_t **B = args->B;
    uint8_t **distances = args->distances;
    int m = args->m;
    int n = args->n;
    int l = args->l;
    int threads = args->total_threads;
    int tid = args->tid;
    unsigned long long int *sums = args->sums;

    for (int i = tid; i < m; i += threads) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < l; k++) {
                if (A[i][k] != B[j][k]) {
                    distances[i][j]++;
                    sums[tid]++;
                }
            }
        }
    }
    pthread_exit(NULL);
}
