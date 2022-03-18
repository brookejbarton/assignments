#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>

int main(int argc, char** argv) {
  struct ppm_pixel *pic;
  int w = 0;
  int h = 0;

  pic = read_ppm(argv[1], &w, &h); 

  printf("Testing file %s: %d %d\n", argv[1], w, h); 

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

