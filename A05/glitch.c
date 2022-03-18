#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>
int main(int argc, char** argv) {
  int w = 0;
  int h = 0;

  struct ppm_pixel *pxs = read_ppm(argv[1], &w, &h); 
  
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
     pxs[i*w + j].red = pxs[i*w + j].red << (rand()%2);
     pxs[i*w + j].green = pxs[i*w + j].green << (rand()%2);
     pxs[i*w + j].blue = pxs[i*w + j].blue << (rand()%2);
    }
  }
  write_ppm(argv[1], pxs, w, h);

 /** for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      printf("(%d, %d, %d) ", pxs[i*w+j].red, pxs[i*w+j].green, 
            pxs[i*w+j].blue);
    }
    printf("\n");
  }*/

  free(pxs);
  pxs = NULL;
  return 0;
}
