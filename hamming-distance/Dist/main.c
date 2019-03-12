#include <string.h>
#include "utils.h"

#define numberOfStringsA 2000
#define numberOfStringsB 2000
#define lenght 100

int main() {
	
	int numOfStringsA, numOfStringsB, len;            //Input args
    uint8_t **A, **B, **distances;                    //Arrays for strings
    long long unsigned *sums;                         //Array for sum of distances

	numOfStringsA = numberOfStringsA;
	numOfStringsB = numberOfStringsB;
    len = lenght;
    
    srand(0);
	
	//SERIAL
	init_arrays(&A, numOfStringsA, &B, numOfStringsB, &distances, len, &sums, 0);
	compute_serial(A, numOfStringsA, B, numOfStringsB, distances, len);
	free_arrays(&A, numOfStringsA, &B, numOfStringsB, &distances, len, &sums);

	//PARALLEL
	for (int threads = 2; threads < 9; threads+=2){
		init_arrays(&A, numOfStringsA, &B, numOfStringsB, &distances, len, &sums, threads);
		compute_pthreads(A, numOfStringsA, B, numOfStringsB, distances, len, sums, threads);
		free_arrays(&A, numOfStringsA, &B, numOfStringsB, &distances, len, &sums);
			
	}
	getchar();
    return 0;
}
