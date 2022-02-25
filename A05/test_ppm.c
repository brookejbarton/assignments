#include <stdio.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  struct ppm_pixel *pic;
  int w = 0;
  int h = 0;

  pic = read_ppm(argv[1], &w, &h);
  
  printf("%s\n", pic->colors);
  return 0;
}

