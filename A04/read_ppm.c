#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"
#include <stdbool.h>

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
 
 if (filename == NULL){
    printf("Error: unable to open file %s\n", filename);
    exit(1);
  }
  
  struct ppm_pixel *matrx = malloc(sizeof(ppm_pixel *) * (h*w));
  char *minimatrx =  malloc(sizeof(char)*1024);
  char *buffer = malloc(sizeof(char)*1024);
  char *token;
  char *whitespace = "  \r";
  bool not_header = false;
  int pos = 0;

  if (minimatrx == NULL || buffer == NULL || token == NULL){
    printf("ERROR: malloc failed!\n");
    exit(1);
  }

  while (buffer != NULL){
    buffer = fgets(buffer, 1024*sizeof(char), *filename);
    token = strtok(buffer, whitespace);
      
    while (token != NULL){
      if (strlen(token) == 3 && not_header == true){
        for (int i = 0; i < strlen(token); i++){
          if (i%3 == 0){
            pos+=1;
          }
          minimatrx[pos] = *token[i];
        }
        strcat(matrx, minimatrx);
      } else {
        not_header = true;
        token = strtok(NULL, whitespace);
      }
    }
  }
  
  
  
  return NULL;
}

