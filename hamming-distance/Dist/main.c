#include <string.h>
#include "utils.h"

#define numberOfStringsA 2000
#define numberOfStringsB 2000
#define lenght 100

int main() {
	
    uint8_t **A, **B, **distances;                    //Arrays for strings
    long long unsigned *sums;                         //Array for sum of distances

    srand(0);

	//PARALLEL
	for (int threads = 2; threads < 9; threads+=2){
		init_arrays(&A, numberOfStringsA, &B, numberOfStringsB, &distances, lenght, &sums, threads);
		compute_pthreads(A, numberOfStringsA, B, numberOfStringsB, distances, lenght, sums, threads);
		free_arrays(&A, numberOfStringsA, &B, numberOfStringsB, &distances, lenght, &sums);
			
	}



	//SERIAL
	init_arrays(&A, numberOfStringsA, &B, numberOfStringsB, &distances, lenght, &sums, 0);
	compute_serial(A, numberOfStringsA, B, numberOfStringsB, distances, lenght);
	free_arrays(&A, numberOfStringsA, &B, numberOfStringsB, &distances, lenght, &sums);

	getchar();
    return 0;
}
