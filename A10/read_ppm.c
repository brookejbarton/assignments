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
     printf("BAD");
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

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
 FILE *fp = fopen(filename, "wb");
  if (fp == NULL){
    printf("ERROR: unable to open file.\n");
    exit(1);
  }
  
  fprintf(fp, "P6\n");
  fprintf(fp, "# comments\n");
  fprintf(fp, "%d %d\n", w, h);
  fprintf(fp, "%d\n", 255);
  fwrite(pixels, sizeof(struct ppm_pixel), w*h, fp);

  fclose(fp);
}

