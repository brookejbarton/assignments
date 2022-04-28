#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include <string.h>
#include <sys/mman.h>

void mandelbrot(int start_col, int end_col, int start_row, int end_row, struct ppm_pixel *pallet, struct ppm_pixel *to_pass, 
                float xmin, float xmax, float ymin, float ymax, int maxIterations, int size){
  // compute image
  for (int i = start_col; i < end_col; i++){ 
    for (int j = start_row; j < end_row; j++){
      float xfrac = (float)(j) / size;
      float yfrac = (float)(i) / size;

      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0; 
    
      while (iter < maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < maxIterations){ // escaped
        to_pass[i*size+j].red = pallet[iter].red;
        to_pass[i*size+j].green = pallet[iter].green;
        to_pass[i*size+j].blue = pallet[iter].blue;
      } else { 
        to_pass[i*size+j].red = 0;
        to_pass[i*size+j].green = 0;
        to_pass[i*size+j].blue = 0;
      } 
    }
  }
}

int main(int argc, char* argv[]) {
  int size = 2000;
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

  int start_col = 0;
  int end_col = size/2;

  int start_row = 0;
  int end_row = size/2;

  //getting shared memory
  int shmid;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel)*(size)*(size), 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel *to_pass = shmat(shmid, NULL, 0);
  if (to_pass == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 

  for (int i = 0; i < numProcesses; i++){
    int pid = fork();
    if (pid == 0) {
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), start_col, end_col, start_row, end_row);
      mandelbrot(start_col, end_col, start_row, end_row, pallet, to_pass, xmin, xmax, ymin, ymax, maxIterations, size);  
      free(pallet);
      pallet = NULL;
      exit(0);
    } else {
      printf("Launched child process: %d\n", pid);
    }

    if (i == 0){
      start_col = end_col;
      end_col+=(size/2);
    }
    if (i == 1){
      start_col = 0;
      end_col = (size/2);

      start_row = end_row;
      end_row+=(size/2);
    }
    if (i == 2){
      start_col = end_col;
      end_col+=(size/2);
    }
  }

  for (int i = 0; i < numProcesses; i++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  
  int timestamp = time(0);
  char name1[] = "multi-mandelbrot-";
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

  if (shmdt(to_pass) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }

  free(pallet);
  pallet = NULL;
}
