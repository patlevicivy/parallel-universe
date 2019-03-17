#include "utils.h"

void compute_serial(uint8_t **A, int numOfStringsA, uint8_t **B, int numOfStringsB, uint8_t **distances, int len) {
    long long unsigned sum = 0;
    double start_time = gettime();
	for (int i = 0; i < numOfStringsA; i++) {
		for (int j = 0; j < numOfStringsB; j++) {
            for (int k = 0; k < len; k++) {
                if (A[i][k] != B[j][k]) {
                    distances[i][j]++;
                    sum++;
                }
            }
        }
    }
	printf("serial  \ttime:%lf\tsum:%llu\n", (gettime() - start_time)/CLOCKS_PER_SEC,sum);
}