#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  struct ppm_pixel *ret_pix;
  char buffer[128];

  
  FILE *fp = fopen(filename, "rb");
  
  
  fgets(buffer, 128, fp);
  if (buffer[0] != 'P' || buffer[1] != '6'){
    printf("ERROR: wrong kind of file.");
    exit(1);
  }
  
  fgets(buffer, 128, fp);
  if (buffer[0] == '#'){
    while (buffer[0] == '#'){
      fgets(buffer, 128, fp);
    }
  }
  
  sscanf(buffer, "%d %d", w, h);
  ret_pix = malloc(sizeof(struct ppm_pixel *)*(*w)*(*h));

  fgets(buffer, 128, fp);  
  fread(ret_pix, sizeof(struct ppm_pixel), (*w)*(*h), fp);
  fclose(fp);
  
  return ret_pix;
}

extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h) {
  FILE *fp = fopen(filename, "wb");
  int *wh = malloc(sizeof(int)*3);
  wh[0] = w;
  wh[1] = h;
  char newColors[3] = {pxs->red << (rand()%2), pxs->green << (rand()%2),
                       pxs->blue << (rand()%2)};

  fwrite("P6\n", 1, 3, fp);
  fwrite("# glitched ver\n", 1, 15, fp);
  fwrite(wh, sizeof(int), 1, fp);
  fwrite(" ", 1, 1, fp);
  fwrite(wh, sizeof(int), 1, fp);
  fwrite("\n", 1, 1, fp);
  fwrite(newColors, sizeof(char), 3, fp); 

  fclose(fp);
}
