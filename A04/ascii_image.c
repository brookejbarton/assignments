#include <stdio.h>
#include "read_ppm.h"
#include "stdlib.h"

int main(int argc, char** argv) {
  struct ppm_pixel *pic;
  int w = 0;
  int h = 0;
  
  pic = read_ppm(argv[1], &w, &h);

  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      float num = (float)(pic[i*w+j].red + pic[i*w+j].green + pic[i*w+j].blue);
      float intensity = num/3.0;
      if (intensity <= 25) printf("@");
      else if (intensity > 25 && intensity <= 50) printf("#");
      else if (intensity > 50 && intensity <= 75) printf("%%");
      else if (intensity > 75 && intensity <= 100) printf("*");
      else if (intensity > 101 && intensity <= 125) printf("o");
      else if (intensity > 125 && intensity <= 150) printf(";");
      else if (intensity > 150 && intensity <= 175) printf(":");
      else if (intensity > 175 && intensity <= 200) printf(",");
      else if (intensity > 200 && intensity <= 225) printf(".");
      else if (intensity > 225 && intensity <= 255) printf(" ");
     // printf(" %f ", intensity);
    }
    printf("\n");
  }

  free(pic);
  pic = NULL;

  return 0;
}

