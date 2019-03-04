#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#define NUM_THREADS 2         //number of threads
#define TOT_COUNT 10000      //total number of iterations
#if defined(__APPLE__)
#include <mach/mach_time.h>
#endif

/**
A random number generator.
Guidance from from http://stackoverflow.com/a/3067387/1281089
**/
float randNumGen(){
   int random_value = rand(); //Generate a random number
   float unit_random = random_value / (float) RAND_MAX; //make it between 0 and 1
   return unit_random;
}

/**
The task allocated to a thread
**/
void *doCalcs(void *threadid){
   long longTid = (long)threadid;
   int tid = (int)longTid;       //obtain the integer value of thread id

   //using malloc for the return variable in order make
   //sure that it is not destroyed once the thread call is finished
   float *in_count = (float *)malloc(sizeof(float));
   *in_count = 0;

   //get the total number of iterations for a thread
   float tot_iterations = TOT_COUNT/NUM_THREADS;

   int counter = 0;

   //calculation
   for(counter = 0; counter < tot_iterations; counter++){
      float x = randNumGen();
      float y = randNumGen();

      float result = sqrt((x * x) + (y * y));

      if(result <= 1){
         *in_count += 1;         //check if the generated value is inside a unit circle
      }

   }

   //get the remaining iterations calculated by thread 0
   if(tid == 0){
      float remainder = TOT_COUNT % NUM_THREADS;

      for(counter = 0; counter < remainder; counter++){
      float x = randNumGen();
      float y = randNumGen();

      float result = sqrt((x * x) + (y * y));

      if(result <= 1){
         *in_count += 1;         //check if the generated value is inside a unit circle
      }

      }
   }


   //printf("In count from #%d : %f\n",tid,*in_count);
   pthread_exit((void *)in_count);     //return the in count
}

void serialTimed(){
  srand ( time(NULL) );         //initiate random seed
  float in_count=0;
  float tot_count=0;
  int tot_iterations=TOT_COUNT;
  int counter=0;

  #if defined(__APPLE__)
     // Time elapsed
     uint64_t t1, t2;
     float duration;
     // Get the timebase info
     mach_timebase_info_data_t info;
     mach_timebase_info(&info);
  #endif

  // Calculate time taken by a request
  #if defined(__APPLE__)
     // Initialize time process
     t1 = mach_absolute_time();
  #endif

  for(counter=0;counter<tot_iterations;counter++){
     float x = randNumGen();
     float y = randNumGen();

     float result = sqrt((x*x) + (y*y));

     if(result<1){
        in_count+=1;         //check if the generated value is inside a unit circle
     }
     tot_count+=1;

  }

  float pi=4*in_count/tot_count;

  // Calculate time it took
  #if defined(__APPLE__)
     // Finished time
     t2 = mach_absolute_time();
     // Time elapsed
     duration = t2 - t1;
    // Convert to seconds
    duration *= info.numer;
    duration /= info.denom;
    duration /= 1000000000;
  #endif


  #if defined(__APPLE__)
     printf("# Elapsed Time = %d seconds %d micro\n", (int)duration, (int)((duration-(int)(duration))*1000000));
     printf("TIME: %f s\n", duration);
  #endif
    //printf( "[SERIAL]   Time taken:            %lf s\n", seconds);
    printf( "[SERIAL]   The value of Pi is:    %f\n",pi);
}

void pthreadTimed(){
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;
  void *status;
  float tot_in = 0;

  clock_t start = clock();

  for(t = 0; t < NUM_THREADS; t++){
    rc = pthread_create(&threads[t], NULL, doCalcs, (void *)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
      }
    }

  //join the threads
  for(t = 0; t < NUM_THREADS; t++){

     pthread_join(threads[t], &status);
     //printf("Return from thread %ld is : %f\n",t, *(float*)status);

     tot_in+= *(float*)status;            //keep track of the total in count

    }

    clock_t end = clock();
    // Calculate time it took
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf( "[PARALEL]  Time taken:            %lf s\n", seconds );
    printf( "[PARALEL]  The Value of Pi is:    %f\n", 4*(tot_in/TOT_COUNT));

  /* Last thing that main() should do */
  pthread_exit(NULL);
}

int main(int argc, char *argv[]){

  serialTimed();
  pthreadTimed();

  exit(0);
}
