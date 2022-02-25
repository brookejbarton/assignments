#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>
int main(int argc, char** argv) {
  int w = 0;
  int h = 0;

  struct ppm_pixel *pxs = read_ppm(argv[1], &w, &h);
 // printf("%s", pxs->colors);  
  write_ppm(argv[1], pxs, w, h);
  return 0;
}
