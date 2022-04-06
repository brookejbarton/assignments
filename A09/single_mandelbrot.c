#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include <string.h>

int main(int argc, char* argv[]) {
  int size = 100;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // generate pallet
  srand(time(0));
  struct ppm_pixel *pallet = calloc(maxIterations, sizeof(struct ppm_pixel)); //read_ppm(argv[1], &w, &h); 
  
  int basered = rand() % 255;
  int basegreen = rand() % 255;
  int baseblue = rand() % 255;
  for (int i = 0; i < maxIterations; i++){
    pallet[i].red = basered + rand() % 100 - 50; //rand() % 255;
    pallet[i].green = basegreen + rand() % 100 - 50; //rand() % 255;
    pallet[i].blue = baseblue + rand() % 100 - 50; //rand() % 255;
 // printf("pallet: r %d, g %d, b %d\n", pallet[i].red, pallet[i].green, pallet[i].blue);
  }

  struct ppm_pixel *to_pass = malloc(sizeof(struct ppm_pixel)*(size)*(size));
  

  // compute image
  for (int i = 0; i < size; i++){ 
    for (int j = 0; j < size; j++){
      float xfrac = (float)(j) / size;
      float yfrac = (float)(i) / size;
    //  printf("xfrac: %f, yfrac: %f\n", xfrac, yfrac);

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
       // printf("Y: %f, X: %f, ITER: %d\n", y, x, iter);
      }

      if (iter < maxIterations){ // escaped
        to_pass[i*size+j].red = pallet[iter].red;
        to_pass[i*size+j].green = pallet[iter].green;
        to_pass[i*size+j].blue = pallet[iter].blue;
     
       //printf("color: %d, %d, %d", to_pass[i*size+j].red, to_pass[i*size+j].green, to_pass[i*size+j].blue);
      } else { 
        to_pass[i*size+j].red = 0;
        to_pass[i*size+j].green = 0;
        to_pass[i*size+j].blue = 0;

     //printf("\nBLACK\n");
     } 
    }
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  
  int timestamp = time(0);
  char name1[] = "mandelbrot-";
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

  free(to_pass);
  to_pass = NULL;
  free(pallet);
  pallet = NULL;
}
