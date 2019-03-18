#include "pthread.h"
#include "utils.h"

void compute_pthreads(uint8_t **A, int numOfStringsA, uint8_t **B, int numOfStringsB, uint8_t **distances,
                      int len, long long unsigned *sums, int threads) {

	//Thread and its agruments are allocated
    //TODO: create and allocate variable threads
    t_args **argTable = (t_args **) malloc(threads * sizeof(t_args *));

	//Argument table is created
    for (int i = 0; i < threads; i++) {
		argTable[i] = construct_targ(A, B, distances, numOfStringsA, numOfStringsB, len, sums, -1, threads);
    }


    double start_time = gettime();

	//Set table id and create thread
    for (int t = 0; t < threads; t++) {
        argTable[t]->tid = t;
        
        
                if (//TODO: create thread that runs task with given arguments
                ) {
                    printf("Error creating thread C with tid=%d\n", t);
                    assert(0);
                }
    }

	//Join threads and free argument table
    for (int t = 0; t < threads; t++) {
        //TODO: wait for the thread t to terminate
        free(argTable[t]);
    }

    free(TT);

	//Sum all distances
    long long unsigned sum = eval_threads_sum(sums, threads);

	double end_time = gettime();
	double final_time = (end_time - start_time) / CLOCKS_PER_SEC;

	printf("threads:%d\ttime:%lf\tsum:%llu\n", threads, final_time,sum);
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
	
    //TODO: terminate thread
}
