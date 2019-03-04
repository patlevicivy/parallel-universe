/* Parallel computation of Pi with Monte Carlo method */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

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

// Thread function
void *my_thread_process(void* threadarg) {
   long int i, j;
   /*register*/ double x, y, z;
   /*register*/ long int localsum = 0;
   struct thread_data* my_data;
   unsigned int rand_state;
   my_data = (struct thread_data*) threadarg;
   rand_state = rdtsc();
   // Generate random values
   for (i=0; i<(my_data->nb_iter)/(my_data->nb_thread); i++) {
      x = (double) rand_r(&rand_state) / RAND_MAX;
      y = (double) rand_r(&rand_state) / RAND_MAX;
      z = x*x + y*y;
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

// Random time cycle
int rdtsc() {
   __asm__ __volatile__("rdtsc");
}

int main (int argc, char** argv) {

   // Total number of hits
   // long int type since may be over 2^32
   long int nb_iter;
   long int i,j;
   // Number of threads
   int nb_process;
   double pi = 0;

   // Posix and time variables
   struct thread_data* ptr;
   // the shared variable
   volatile double s=0;
   pthread_mutex_t s_lock;
   pthread_t* ptr_tid;
   pthread_attr_t attr;
   void *ret;

   // Check number of arguments
   if (argc != 3) {
     printf("Error: specify arguments as number of threads and iterations\n");
     exit(1);
   }
   // Assign arguments
   nb_process = atoi(argv[1]);
   nb_iter = atol(argv[2]);
   // Check divisibility
   if (nb_iter % nb_process != 0) {
     printf("Error: number of iterations not divisible by number of threads\n");
     exit(1);
   }

   // nb_process pthreads allocation
   ptr_tid = (pthread_t*) calloc(nb_process, sizeof(pthread_t));
   ptr = (struct thread_data*) calloc(nb_process, sizeof(struct thread_data));
   // Initialize the lock variable
   pthread_mutex_init(&s_lock, NULL);

   clock_t start = clock();

   for (i=0; i<nb_process; i++) {
      ptr[i].p_s = &s;
      ptr[i].p_s_lock = &s_lock;
      ptr[i].nb_thread = nb_process;
      ptr[i].nb_iter = nb_iter;
      pthread_attr_init(&attr);
      pthread_create(&ptr_tid[i], &attr, my_thread_process,&ptr[i]);
   }

   // Join nb_process pthreads
   for (j=0; j<nb_process; j++)
      pthread_join(ptr_tid[j], &ret);

  clock_t end = clock();

   // Pi value
   pi = s/nb_iter;
   pi *= 4;
   // Output
   printf("# Estimation of Pi = %1.8f\n", pi);
   printf("# Number of tries = %ld\n", nb_iter);

   float seconds = (float)(end - start) / CLOCKS_PER_SEC;
   printf( "# Time taken: %lf s\n", seconds);

   return 0;
}
