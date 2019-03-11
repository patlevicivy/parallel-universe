#include "utils.h"

void compute_serial(uint8_t **A, int m, uint8_t **B, int n, uint8_t **distances, int l) {
    long long unsigned sum = 0;
    double start_time = gettime();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < l; k++) {
                if (A[i][k] != B[j][k]) {
                    distances[i][j]++;
                    sum++;
                }
            }
        }
    }
    printf("sum:%llu\ttime:%lf", sum, gettime() - start_time);
}