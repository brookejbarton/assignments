#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"
#include <stdbool.h>

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  struct ppm_pixel *ret_pix;
  char buffer[128];

  
  FILE *fp = fopen(filename, "r");
  
  
  fgets(buffer, 128, fp);
  if (buffer[0] != 'P' || buffer[1] != '3'){
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

