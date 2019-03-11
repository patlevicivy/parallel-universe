#include "utils.h"

#define size 1000
#define lenght 10
#define threadsNUM 2
#define seedNUM 0

int main(int argc, char *argv[]) {
    if (argc < 8) {
        printf("\nNeed more arguments...\nExample: ./run 100 100 10 4 0 pthreads 1");
        return -1;
    }

    int m, n, l, threads, rand_seed;            //Input args
    uint8_t **A, **B, **distances;
    long long unsigned *sums;

    m = size;
    n = size;
    l = lenght;
    threads = threadsNUM;
    rand_seed = seedNUM;
    char *type = "pthreads";   //sequential,pthreads
    int mode = 3;   //1,2,3
    if (m < 1 || n < 1 || l < 1 || threads < 1 || CHUNK < 1 || mode < 0 || mode > 3) {
        printf("\nInvalid arguments..");
        return -1;
    }

    printf("\ntype:%s\tmode:%d\t", type, mode);
    srand(rand_seed);
    init_arrays(&A, m, &B, n, &distances, l, &sums, threads);

    //Serial implementation
    if (strcmp(type, "sequential") == 0) {
        compute_serial(A, m, B, n, distances, l);
    } else if (strcmp(type, "pthreads") == 0) {
        compute_pthreads(A, m, B, n, distances, l, mode, sums, threads);
    } else {
        printf("else clause at type.");
        return -1;
    }

    free_arrays(&A, m, &B, n, &distances, l, &sums);

	

    return 0;
}
