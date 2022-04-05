#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

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

  // todo: your work here
  // generate pallet
  int w = size; //abs(xmax-xmin);
  int h = size ; //abs(ymax-ymin);

  struct ppm_pixel *pallet = calloc(maxIterations, sizeof(struct ppm_pixel)); //read_ppm(argv[1], &w, &h); 
  
  srand(time(0));

  for (int i = 0; i < maxIterations; i++){
    pallet[i].red = rand() % 255;
    pallet[i].green = rand() % 255;
    pallet[i].blue = rand() % 255;
  //  printf("pallet: r %d, g %d, b %d", pallet[i].red, pallet[i].green, pallet[i].blue);
  }
  
  int timestamp = time(0);
  char name1[] = "data/mendelbrot-.png";
  //strcat(name1, size);
  //strcat(name1, "-");
  //strcat(name1, timestamp);
  //strcat(name1, ".png");
  const char *filename = name1;

  if (filename == NULL){
    printf("Unable to create file.\n");
    exit(1);
  }
  // compute image
  int iter = 0;
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      int xfrac = j / size;
      int yfrac = i / size;
      int x0 = xmin + xfrac * (xmax - xmin);
      int y0 = ymin + yfrac * (ymax - ymin);

      int x = 0;
      int y = 0;
      
      struct ppm_pixel *color = calloc(1, sizeof(struct ppm_pixel));

      while (iter < maxIterations && x*x + y*y < 2*2){
        int xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        

        if (iter < maxIterations){ // escaped
          color->red = pallet[iter].red;
          color->green = pallet[iter].green;
          color->blue = pallet[iter].blue;
        } else { //i don't understand how this is meant to be reached, iter will always be less than MAX
          color->red = 0;
          color->green = 0;
          color->blue = 0; //black
        }

      //  printf("r %d, g %d, b%d || ", color->red, color->green, color->blue);
        iter++;
       // write_ppm(filename, color, w, h); //write color to image at location (row,col)
      } 
    }
  }
}
