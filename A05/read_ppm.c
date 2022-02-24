#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  //char buffer[16];
  //char header;
  struct pm_pixel *ret_pix;
  int c, rgb_set;

  ret_pix = malloc(sizeof(struct ppm_pixel *));
  
  FILE *fp = fopen(filename, "rb");
  
  //check if P6
  //fgets(buff, sizeof(buff,), fp);
  //if (buff[0])
  
  c = getc(fp);
  while (c == '#'){
    while (c!='\n'){
      c = getc(fp);
    }
  }
  ungetc(c, fp);
  
  fscanf(fp, "%d %d", w, h);
  fscanf(fp, "%d", &rgb_set);
  
  fread(ret_pix, sizeof(struct ppm_pixel), 3, fp);
  fclose(fp);

 // for (int i = 0; i < 3; i++){
   // printf("ret_pix[%d] = (%c,%c, %c)\n", i, ret_pix[i]->red, ret_pix[i]->green, red_pix[i]->blue);
  //}

   
  
  
  return NULL;
}

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h) {

}
