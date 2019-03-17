/* Parallel computation of Pi with Monte Carlo method */

#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <time.h>
#include <math.h>

#define TOT_COUNT 100000000

// Structure for thread
struct thread_data {
	// Shared value
	double volatile *shared_val;
	// Lock variable
	pthread_mutex_t *lock;
	// Number of iterations
	long int iter_count;
	// Number of threads
	int thread_count;
	// ID redurned by create
	pthread_t thread_id;
};

double rand_num_gen() {
	//Generate a random number
	int random_value = rand();
	//make it between 0 and 1
	double unit_random = random_value / (double)RAND_MAX;
	return unit_random;
}

// Thread function
void *my_thread_process(void *thread_arg) {
	double x, y;
	long int local_sum = 0;
	struct thread_data *my_data;
	my_data = (struct thread_data *) thread_arg;
	// Generate random values
	for (long int i = 0; i < (my_data->iter_count) / (my_data->thread_count); i++) {
		x = rand_num_gen();
		y = rand_num_gen();

		if (sqrt(x * x + y * y) <= 1.0)
			local_sum++;
	}
	// TODO: Assert the lock on shared value

	// Change the value of sum
	*(my_data->shared_val) += local_sum;
	// TODO: Remove the lock

	// TODO: Terminate the thread

	return NULL;
}

void parallel(int nb_process) {
	double pi = 0;
	// Posix variables
	struct thread_data *ptr;
	// the shared variable
	volatile double sum = 0;
	pthread_mutex_t sum_lock;

	// Allocation
	ptr = (struct thread_data *) calloc(nb_process, sizeof(struct thread_data));
	// TODO: Initialize the lock variable

	clock_t start = clock();

	for (long int i = 0; i < nb_process; i++) {
		ptr[i].shared_val = &sum;
		ptr[i].lock = &sum_lock;
		ptr[i].thread_count = nb_process;
		ptr[i].iter_count = TOT_COUNT;

		if () { // TODO: Start a new thread and check return value
			printf("Error creating thread with id = %ld\n", i);
			exit(EXIT_FAILURE);
		}
	}

	// Join nb_process pthreads
	for (long int j = 0; j < nb_process; j++)
		if () { // TODO: Wait for the thread specified by thread_id to terminate and check return value
			printf("Error joining thread with id = %ld\n", j);
			exit(EXIT_FAILURE);
		}

	// Pi value
	pi = 4 * sum / TOT_COUNT;

	// Output
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("[PARALLEL]   # Time taken: %lf s\n", seconds);
	printf("             # Estimation of Pi = %lf\n", pi);
	printf("             # Number of tries = %d\n", TOT_COUNT);
	printf("             # Number of threads = %d\n\n", nb_process);

	free(ptr);
}

void sequential() {
	double volatile x, y;
	double volatile sum = 0;

	// Calculate time taken by a request
	clock_t start = clock();

	// Generate random values
	for (long int i = 0; i < TOT_COUNT; i++) {
		x = rand_num_gen();
		y = rand_num_gen();

		if (sqrt(x * x + y * y) <= 1.0)
			sum++;
	}

	double pi = 4 * sum / TOT_COUNT;

	// Calculate time it took
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("[SEQUENTIAL] # Time taken: %lf s\n", seconds);
	printf("             # Estimation of Pi = %lf\n", pi);
	printf("             # Number of tries = %d\n\n", TOT_COUNT);

}

int main(int argc, char **argv) {
	sequential();

	parallel(2);
	parallel(4);
	parallel(8);

	getchar();

	exit(EXIT_SUCCESS);
}
