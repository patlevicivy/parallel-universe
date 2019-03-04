/* Parallel computation of Pi with Monte Carlo method */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#if defined(__APPLE__)
#include <mach/mach_time.h>
#endif

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

float randNumGen(){
   int random_value = rand(); //Generate a random number
   float unit_random = random_value / (float) RAND_MAX; //make it between 0 and 1
   return unit_random;
}

// Thread function
void *my_thread_process(void* threadarg) {
   long int i, j;
   /*register*/ double x, y, z;
   /*register*/ long int localsum = 0;
   struct thread_data* my_data;
   unsigned int rand_state;
   my_data = (struct thread_data*) threadarg;
   //rand_state = rdtsc();
   // Generate random values
   for (i=0; i<(my_data->nb_iter)/(my_data->nb_thread); i++) {
      //x = (double) rand_r(&rand_state) / RAND_MAX;
      //y = (double) rand_r(&rand_state) / RAND_MAX;
      x =( double) randNumGen();
      y = (double) randNumGen();

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
#if defined(__APPLE__)
   // Time elapsed
   uint64_t t1, t2;
   float duration;
   // Get the timebase info
   mach_timebase_info_data_t info;
   mach_timebase_info(&info);
#elif defined(__linux__)
   struct timeval chrono1, chrono2;
   int micro, second;
#endif
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

#if defined(__APPLE__)
   // Initialize time process
   t1 = mach_absolute_time();
#elif defined(__linux__)
   // Initialize time process
   gettimeofday(&chrono1, NULL);
      // Loop pthreads creation
#endif

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

#if defined(__APPLE__)
   // Finished time
   t2 = mach_absolute_time();
   // Time elapsed
   duration = t2 - t1;
  // Convert to seconds
  duration *= info.numer;
  duration /= info.denom;
  duration /= 1000000000;
#elif defined(__linux__)
   // Finished time
   gettimeofday(&chrono2, NULL);
   // Compute ellapsed time
   micro = chrono2.tv_usec - chrono1.tv_usec;
   if (micro < 0) {
     micro += 1000000;
     second = chrono2.tv_sec - chrono1.tv_sec - 1;
   }
   else second = chrono2.tv_sec - chrono1.tv_sec;
#endif

   // Pi value
   pi = s/nb_iter;
   pi *= 4;
   // Output
   printf("# Estimation of Pi = %1.8f\n", pi);
   printf("# Number of tries = %ld\n", nb_iter);

#if defined(__APPLE__)
   printf("# Elapsed Time = %d seconds %d micro\n", (int)duration, (int)((duration-(int)(duration))*1000000));
   printf("TIME: %f s\n", duration);
#elif defined(__linux__)
   printf("# Elapsed Time = %d seconds %d micro\n", second, micro);
#endif

   return 0;
}
