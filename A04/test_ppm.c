#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>

int main() {
  struct ppm_pixel *pic;
  int w = 4;
  int h = 4;
  const char *filename = "feep-ascii.ppm";
  pic = read_ppm(filename, &w, &h); 

  printf("Testing file %s: %d %d\n", filename, w, h); 

  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      printf("(%d, %d, %d) ", pic[i*w+j].red, pic[i*w+j].green, pic[i*w+j].blue);
    }
    printf("\n");
  }

  free(pic);
  pic = NULL;

  return 0;
}

