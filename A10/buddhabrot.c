#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>

int max = 0;
pthread_mutex_t mutex;
pthread_barrier_t barrier;

struct mandel_pack {
  int start_col;
  int end_col; 
  int start_row; 
  int end_row;
  struct ppm_pixel *to_pass;
  bool *escaped;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int size;
  int *count;
};

void color_comp(struct mandel_pack *pack){
  float gamma = 0.681;
  float factor = 1.0/gamma;

  for (int i = pack->start_col; i < pack->end_col; i++){ 
    for (int j = pack->start_row; j < pack->end_row; j++){
        float value = 0;
        
        if (pack->count[i*pack->size+j] > 0) {
          value = log((float)(pack->count[i*pack->size+j])) / log(max);
          value = pow(value, factor);
        }

        pack->to_pass[i*pack->size+j].red = value * 255;
        pack->to_pass[i*pack->size+j].green = value * 255;
        pack->to_pass[i*pack->size+j].blue = value * 255;
    }
  }
}

void visit_counts(struct mandel_pack *pack){
  for (int i = pack->start_col; i < pack->end_col; i++){ 
    for (int j = pack->start_row; j < pack->end_row; j++){
      if (pack->escaped[i*pack->size+j] == false){
        float xfrac = (float)(j) / pack->size;
        float yfrac = (float)(i) / pack->size;

        float x0 = pack->xmin + xfrac * (pack->xmax - pack->xmin);
        float y0 = pack->ymin + yfrac * (pack->ymax - pack->ymin);

        float x = 0;
        float y = 0;

        while (x*x + y*y < 2*2){
          float xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;

          float yrow = round(pack->size * (y - pack->ymin)/(pack->ymax - pack->ymax));
          float xcol = round(pack->size * (x - pack->xmin)/(pack->xmax - pack->xmax));
          
          if (yrow < 0 || yrow >= pack->size){ // out of range
            if (xcol < 0 || xcol >= pack->size) { // out of range  
              pthread_mutex_lock(&mutex);
              pack->count[i*pack->size+j]++;
              pthread_mutex_unlock(&mutex);

              if (pack->count[i*pack->size+j] > max){
                max = pack->count[i*pack->size+j];
              }
            }
          }
        }
      }
    }
  }
}

void *mandelbrot(void *args){
  struct mandel_pack *pack = (struct mandel_pack *)args;
  printf("Thread %ld) sub-image block: cols (%d, %d) to rows (%d, %d)\n", 
        pthread_self(), pack->start_col, pack->end_col, pack->start_row, pack->end_row);
  
  // compute image
  for (int i = pack->start_col; i < pack->end_col; i++){ 
    for (int j = pack->start_row; j < pack->end_row; j++){
      float xfrac = (float)(j) / pack->size;
      float yfrac = (float)(i) / pack->size;

      float x0 = pack->xmin + xfrac * (pack->xmax - pack->xmin);
      float y0 = pack->ymin + yfrac * (pack->ymax - pack->ymin);

      float x = 0;
      float y = 0;
      int iter = 0; 
    
      while (iter < pack->maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < pack->maxIterations){ // escaped
       pack->escaped[i*pack->size+j] = false;
      } else { 
       pack->escaped[i*pack->size+j] = true;
      } 
    }
  }

  visit_counts(pack);

  pthread_barrier_wait(&barrier);
  color_comp(pack);
  
  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);
  printf("\n");

  srand(time(0));

  pthread_t *thread_array = malloc(numProcesses * sizeof(pthread_t));
  struct ppm_pixel *to_pass = malloc(sizeof(struct ppm_pixel)*(size)*(size));
  struct mandel_pack *q_pack = calloc(numProcesses, sizeof(struct mandel_pack));

  for (int i = 0; i < numProcesses; i ++){
    q_pack[i].xmin = -2.0;
    q_pack[i].xmax = 0.47;
    q_pack[i].ymin = -1.12;
    q_pack[i].ymax = 1.12;
    q_pack[i].maxIterations = 1000;
    q_pack[i].size = 480;
    q_pack[i].escaped = calloc((size)*(size), sizeof(bool));
    q_pack[i].count = calloc((size)*(size), sizeof(int)); //calloc(sizeof(int)*(size)*(size));
  }

  //quadrant 1
    q_pack[0].start_col = 0;
    q_pack[0].end_col = 240;
    q_pack[0].start_row = 0;
    q_pack[0].end_row = 240;
    q_pack[0].to_pass = to_pass;

  //quadrant 2
    q_pack[1].start_col = 240;
    q_pack[1].end_col = 480;
    q_pack[1].start_row = 0;
    q_pack[1].end_row = 240;
    q_pack[1].to_pass = to_pass;

  //quandrant 3
    q_pack[2].start_col = 0;
    q_pack[2].end_col = 240;
    q_pack[2].start_row = 240;
    q_pack[2].end_row = 480;
    q_pack[2].to_pass = to_pass;

  //quadrant 4 
    q_pack[3].start_col = 240;
    q_pack[3].end_col = 480;
    q_pack[3].start_row = 240;
    q_pack[3].end_row = 480;
    q_pack[3].to_pass = to_pass;

  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, 4);

  for (int i = 0; i < numProcesses; i++){
    if (pthread_create(&thread_array[i], NULL, &mandelbrot, (void *)&q_pack[i]) != 0){
      printf("ERROR: thread q%d could not be created.\n", i);
      exit(1);
    }
  }

  for (int i = 0; i < numProcesses; i++){
    if (pthread_join(thread_array[i], NULL) != 0){
      printf("ERROR: thread q%d could not be joined.\n", i);
      exit(1);
    } else {
      printf("Thread) %ld finished\n", thread_array[i]); 
    }
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  printf("\nComputed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  
  int timestamp = time(0);
  char name1[] = "buddhabrot-";
  char sizestr[20];
  sprintf(sizestr, "%d", size);
  strcat(name1, sizestr);
  strcat(name1,"-");
  char timestr[20];
  sprintf(timestr, "%d", timestamp);
  strcat(name1, timestr);
  strcat(name1, ".ppm");
  const char *filename = name1;

  if (filename == NULL){
    printf("Unable to create file.\n");
    exit(1);
  }

  printf("Writing file: %s\n", name1);

  write_ppm(filename, to_pass, size, size);

  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);

  for (int i = 0; i < numProcesses; i++){ 
    for (int j = 0; j < numProcesses; j++){
      free(q_pack[i].escaped);
      q_pack[i].escaped = NULL;

      free(q_pack[i].count);
      q_pack[i].count = NULL;
    }
  }

  free(to_pass);
  to_pass = NULL;
  free(thread_array);
  thread_array = NULL;
  free(q_pack);
  q_pack = NULL;
}
