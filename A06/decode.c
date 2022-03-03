#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*char* convert_bit(unsigned char color, char *bits){
  char odd = '1';
  char even = '0';

  if (color%2!=0){
    strcat(bits, &odd);
  } else {
    strcat(bits, &even);
  }

  return bits;
}*/

char* bit_to_ASCII(char bits[72]){ //param will be char*
  char binstr[8];
  char *ascii = malloc(sizeof(char)*73);//*(sizeof(bits)/8)+1);
  bool b = true;
  int count = 0; 
  int shift = 0;
  
 /* if (sizeof(bits)%8!=0){
    printf("ERROR: Not Binary");
    exit(1);
  }*/
   
  while (b == true){
    for (int i = 0; i < 8; i++){
      binstr[i] = bits[i+shift];
    }
    
    for (int i = 0; i < 8; i++){
      if (binstr[i] == 1){
        count+=1;
      }
    }

    if (count == 0){
      return ascii;
    }
  
    char c = strtol(binstr, (char **)NULL, 2);
    strcat(ascii, &c);
    count = 0;
    shift+=8;
  }
  return ascii;
}

int main(int argc, char** argv) {
  struct ppm_pixel *rgb;
  int w = 0;
  int h = 0;
  int count = 0;
  rgb = read_ppm(argv[1], &w, &h);

  unsigned char *bits = malloc((sizeof(int)*(w*h)+1));
  memset(bits, 0, (w*h)); 
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      int idx = i*w+j;
      bits[count] = rgb[idx].red;
      bits[count+1] = rgb[idx].green;
      bits[count+2] = rgb[idx].blue;
      count++;
      printf("%d, %d, %d", rgb[idx].red, rgb[idx].green, rgb[idx].blue);
    }
    printf("\n");
  }

  printf("bits: %s", bits);
  char practice[72] = {0,0,1,1,0,0,0,1,0,0,1,1,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0};
  char *ascii = bit_to_ASCII(practice);
  printf("%s", ascii);
  return 0;
}

