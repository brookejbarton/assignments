#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000

struct args {
  int dotproduct;
  int v;
  int u;
};

void *routine(void *args){
  struct args *info = (struct args *)args;
  //srand(time(0));
 // int v[SIZE];
 // int u[SIZE];
 // int dotproduct = info->dotproduct;

  for (int i = 0; i < SIZE/4; i++) {
   // v[i] = rand() % 1000 - 500;
   // u[i] = rand() % 1000 - 500;
    info->dotproduct += info->u * info->v;
  } 

  printf("Product: %d\n", info->dotproduct);

  return NULL;
}

int main(int argc, char *argv[]) {
  
  srand(time(0));

  int v[SIZE];
  int u[SIZE];
  int dotproduct = 0;

  for (int i = 0; i < SIZE; i++) {
    v[i] = rand() % 1000 - 500;
    u[i] = rand() % 1000 - 500;
    dotproduct += u[i] * v[i];
  }
  printf("Ground truth dot product: %d\n", dotproduct);

  pthread_t *thread_array = malloc(SIZE * sizeof(pthread_t));
  struct args info[4];

  for (int i = 0; i < 4; i++){
    info[i].dotproduct = 0;
    for (int j = 0; j < SIZE/4; j++){
      info[i].v += v[j];
      info[i].u += u[j];
    } 
  }
  
  printf("Test with 4 threads\n");
  for (int i = 0; i < 4; i++){
    pthread_create(&thread_array[i], NULL, routine, &info[i]);
  }

  for (int i = 0; i < 4; i++){
    pthread_join(thread_array[i], NULL);
  }

  int total = 0;

  for (int i = 0; i < 4; i++){
    total+=info[i].dotproduct;
  }

  printf("Answer with 4 threads: %d\n", total);

  free(thread_array);
  thread_array = NULL;
  
  return 0;
}
