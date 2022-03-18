#include <stdio.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  struct ppm_pixel *pic;
  int w = 0;
  int h = 0;

  pic = read_ppm(argv[1], &w, &h); 

  printf("Testing file %s: %d %d\n", argv[1], w, h); 

  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      printf("(%d, %d, %d) ", pic[i+j].red, pic[i+j].green, pic[i+j].blue);
    }
    printf("\n");
  }
  return 0;
}

