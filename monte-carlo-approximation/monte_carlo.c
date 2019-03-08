/* Parallel computation of Pi with Monte Carlo method */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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
    double unit_random = random_value / (double) RAND_MAX;
    return unit_random;
}

// Thread function
void *my_thread_process(void *threadarg) {
    register double x, y, z;
    register long int localsum = 0;
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    // Generate random values
    for (long int i = 0; i < (my_data->nb_iter) / (my_data->nb_thread); i++) {
        x = rand_num_gen();
        y = rand_num_gen();

        z = x * x + y * y;
        if (z <= 1.0)
            localsum++;
    }
    // Thread asserting the lock on s
    pthread_mutex_lock(my_data->p_s_lock);
    // Change the value of s
    *(my_data->p_s) += localsum;
    // Remove the lock
    pthread_mutex_unlock(my_data->p_s_lock);

    return NULL;
}

void parallel(int nb_process) {
    // Number of threads
    double pi = 0;

    // Posix and time variables
    struct thread_data *ptr;
    // the shared variable
    volatile double s = 0;
    pthread_mutex_t s_lock;
    pthread_t *ptr_tid;
    pthread_attr_t attr;
    void *ret;

    // nb_process pthreads allocation
    ptr_tid = (pthread_t *) calloc(nb_process, sizeof(pthread_t));
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

    clock_t end = clock();

    // Pi value
    pi = s / TOT_COUNT;
    pi *= 4;
    // Output
    float seconds = (float) (end - start) / CLOCKS_PER_SEC;
    printf("[PARALLEL]   # Time taken: %lf s\n", seconds);
    printf("[PARALLEL]   # Estimation of Pi = %lf\n", pi);
    printf("[PARALLEL]   # Number of tries = %d\n", TOT_COUNT);
    printf("[PARALLEL]   # Number of threads = %d\n\n", nb_process);

}

#pragma GCC push_options
#pragma GCC optimize ("O0")

void serial() {
    double volatile x, y, z;
    double volatile sum = 0;

    // Calculate time taken by a request
    clock_t volatile start = clock();

    // Generate random values
    for (int i = 0; i < TOT_COUNT; i++) {
        x = rand_num_gen();
        y = rand_num_gen();

        z = x * x + y * y;
        if (z <= 1.0)
            sum++;
    }

    double volatile pi = sum / TOT_COUNT;
    pi *= 4;

    // Calculate time it took
    clock_t volatile end = clock();
    float volatile seconds = (float) (end - start) / CLOCKS_PER_SEC;
    printf("[SERIAL]     # Time taken: %lf s\n", seconds);
    printf("[SERIAL]     # Estimation of Pi = %lf\n", pi);
    printf("[SERIAL]     # Number of tries = %d\n", TOT_COUNT);

}

#pragma GCC pop_options

int main(int argc, char **argv) {
    parallel(2);
    parallel(4);
    parallel(8);
    serial();

    return 0;
}
