#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#define NUM_THREADS 2         //number of threads
#define TOT_COUNT 100000000      //total number of iterations

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

      if(result < 1){
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

      if(result < 1){
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

  // Calculate time taken by a request
  clock_t start = clock();

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
  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf( "[SERIAL]   Time taken:            %lf s\n", seconds);
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
