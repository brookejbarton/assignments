#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include <sys/mman.h>
#include <string.h>
#include <sys/time.h>
#include <sys/shm.h>

struct mandel_pack {
  int start_col;
  int end_col; 
  int start_row; 
  int end_row;
  struct ppm_pixel *pallet;
  struct ppm_pixel *to_pass;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int size;
};

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
        pack->to_pass[i*pack->size+j].red = pack->pallet[iter].red;
        pack->to_pass[i*pack->size+j].green = pack->pallet[iter].green;
        pack->to_pass[i*pack->size+j].blue = pack->pallet[iter].blue;
      } else { 
        pack->to_pass[i*pack->size+j].red = 0;
        pack->to_pass[i*pack->size+j].green = 0;
        pack->to_pass[i*pack->size+j].blue = 0;
      } 
    }
  }

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
  struct ppm_pixel *pallet = calloc(maxIterations, sizeof(struct ppm_pixel));
  
  int basered = rand() % 255;
  int basegreen = rand() % 255;
  int baseblue = rand() % 255;
  for (int i = 0; i < maxIterations; i++){
    pallet[i].red = basered + rand() % 100 - 50; //rand() % 255;
    pallet[i].green = basegreen + rand() % 100 - 50; //rand() % 255;
    pallet[i].blue = baseblue + rand() % 100 - 50; //rand() % 255;
  }

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
  }

  //quadrant 1
    q_pack[0].start_col = 0;
    q_pack[0].end_col = 240;
    q_pack[0].start_row = 0;
    q_pack[0].end_row = 240;
    q_pack[0].pallet = pallet;
    q_pack[0].to_pass = to_pass;

  //quadrant 2
    q_pack[1].start_col = 240;
    q_pack[1].end_col = 480;
    q_pack[1].start_row = 0;
    q_pack[1].end_row = 240;
    q_pack[1].pallet = pallet;
    q_pack[1].to_pass = to_pass;

  //quandrant 3
    q_pack[2].start_col = 0;
    q_pack[2].end_col = 240;
    q_pack[2].start_row = 240;
    q_pack[2].end_row = 480;
    q_pack[2].pallet = pallet;
    q_pack[2].to_pass = to_pass;

  //quadrant 4 
    q_pack[3].start_col = 240;
    q_pack[3].end_col = 480;
    q_pack[3].start_row = 240;
    q_pack[3].end_row = 480;
    q_pack[3].pallet = pallet;
    q_pack[3].to_pass = to_pass;

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
  char name1[] = "thread-mandelbrot-";
  char sizestr[20] = "";
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

  free(to_pass);
  to_pass = NULL;
  free(pallet);
  pallet = NULL;
  free(thread_array);
  thread_array = NULL;
  free(q_pack);
  q_pack = NULL;
}
