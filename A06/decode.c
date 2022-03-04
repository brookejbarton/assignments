#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void leastSigBit(unsigned char *bits, int* ret, int len){
  for(int i = 0; i < len; i++){
    if (bits[i]%2==0){
      ret[i] = 0;  
    } else {
      ret[i] = 1;
    }
  }
}

int bit_to_ASCII(unsigned char *bits, char *ascii, int len){
  char *binstr = malloc(sizeof(char)*9);
  bool b = true;
  int count = 0; 
  int shift = 0;
  
  int *binBits = malloc(sizeof(int)*(len+1));
  leastSigBit(bits, binBits, len);
  printf("binBits: ");
  for (int i = 0; i < len; i++){  
    printf("%d", binBits[i]);
  }
  printf("\n");

  if (len%8!=0){
    printf("ERROR: Not Binary");
    exit(1);
  }
   
  while (b == true){
    for (int i = 0; i < 8; i++){
      binstr[i] = binBits[i+shift];
    }
    
    for (int i = 0; i < 8; i++){
      if (binstr[i] == 1){
        count+=1; //could replace this with a bool so doesnt have to iterate through the whole thini
      }
    }

    if (count == 0){
      return 0;
    }
    
    char c = strtol(binstr, (char **)NULL, 2);
    printf("char is: %02X\n", c);
    strcat(ascii, &c);
    count = 0;
    shift+=8;
  }
  return 0;
}

int main(int argc, char** argv) {
  struct ppm_pixel *rgb;
  int w = 0;
  int h = 0;
  int count = 0;
  
  if (!argv[1]){
    printf("ERROR: no file given!\n");
    exit(1);
  }
  
  FILE *infile = fopen(argv[1], "r");
  if (infile == NULL){
    printf("Error: unable to read file %s\n", argv[1]);
    exit(1);
  }
  fclose(infile);

  rgb = read_ppm(argv[1], &w, &h);

  unsigned char *bits = malloc((sizeof(int)*(w*h)+1));
  memset(bits, 0, (w*h)); 
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      int idx = i*w+j;
      bits[count] = rgb[idx].red;
      bits[count+1] = rgb[idx].green;
      bits[count+2] = rgb[idx].blue;
      count+=3;
    }
  }
  
  printf("bits: ");
  for (int i = 0; i < count; i++){
    printf("%d", bits[i]);
  }
  printf("\n");
 
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", (count/8)); 
  char *ascii = malloc(sizeof(char)*73);//*(sizeof(bits)/8)+1);
  bit_to_ASCII(bits, ascii, count);
  printf("%s", ascii);
  return 0;
}

