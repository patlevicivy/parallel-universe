/* Parallel computation of Pi with Monte Carlo method */

#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <time.h>
#include <math.h>

#define TOT_COUNT 10000000

// Structure for thread
struct thread_data {
	// Shared value
	double volatile *p_s;
	// Lock variable s
	pthread_mutex_t *p_s_lock;
	// Number of iterations
	long int nb_iter;
	// Number of threads
	int nb_thread;
};

double rand_num_gen() {
	//Generate a random number
	int random_value = rand();
	//make it between 0 and 1
	double unit_random = random_value / (double)RAND_MAX;
	return unit_random;
}

// Thread function
void *my_thread_process(void *threadarg) {
	register double x, y, z;
	register long int local_sum = 0;
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	// Generate random values
	for (long int i = 0; i < (my_data->nb_iter) / (my_data->nb_thread); i++) {
		x = rand_num_gen();
		y = rand_num_gen();

		z = sqrt(x * x + y * y);
		if (z <= 1.0)
			local_sum++;
	}
	// Thread asserting the lock on s
	pthread_mutex_lock(my_data->p_s_lock);
	// Change the value of s
	*(my_data->p_s) += local_sum;
	// Remove the lock
	pthread_mutex_unlock(my_data->p_s_lock);

	return NULL;
}

void parallel(int nb_process) {
	double pi = 0;
	// Posix variables
	struct thread_data *ptr;
	// the shared variable
	volatile double s = 0;
	pthread_mutex_t s_lock;
	pthread_t *ptr_tid;
	pthread_attr_t attr;
	void *ret;

	// nb_process pthreads allocation
	ptr_tid = (pthread_t *)calloc(nb_process, sizeof(pthread_t));
	ptr = (struct thread_data *) calloc(nb_process, sizeof(struct thread_data));
	// Initialize the lock variable
	pthread_mutex_init(&s_lock, NULL);

	clock_t start = clock();

	for (long int i = 0; i < nb_process; i++) {
		ptr[i].p_s = &s;
		ptr[i].p_s_lock = &s_lock;
		ptr[i].nb_thread = nb_process;
		ptr[i].nb_iter = TOT_COUNT;
		pthread_attr_init(&attr);
		pthread_create(&ptr_tid[i], &attr, my_thread_process, &ptr[i]);
	}

	// Join nb_process pthreads
	for (long int j = 0; j < nb_process; j++)
		pthread_join(ptr_tid[j], &ret);

	// Pi value
	pi = s / TOT_COUNT;
	pi *= 4;

	// Output
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("[PARALLEL]   # Time taken: %lf s\n", seconds);
	printf("					   # Estimation of Pi = %lf\n", pi);
	printf("					   # Number of tries = %d\n", TOT_COUNT);
	printf("					   #Number of threads = %d\n\n", nb_process);

}

void sequential() {
	double volatile x, y, z;
	double volatile sum = 0;

	// Calculate time taken by a request
	clock_t start = clock();

	// Generate random values
	for (int i = 0; i < TOT_COUNT; i++) {
		x = rand_num_gen();
		y = rand_num_gen();

		z = sqrt(x * x + y * y);
		if (z <= 1.0)
			sum++;
	}

	double volatile pi = sum / TOT_COUNT;
	pi *= 4;

	// Calculate time it took
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("[SEQUENTIAL] # Time taken: %lf s\n", seconds);
	printf("    				 # Estimation of Pi = %lf\n", pi);
	printf("     				 # Number of tries = %d\n", TOT_COUNT);

}

int main(int argc, char **argv) {
	sequential();

	parallel(2);
	parallel(4);
	parallel(8);

	getchar();

	exit(0);
}
