#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int* toBinary(int dec){
  int tmp = 0;
  int *ret = 0;
  int inc = 1;

  while (dec!=0){
    tmp = dec%2;
    *ret+= (tmp*inc);
    inc*= 10;
    dec /= 2;     
  } 

  return ret; 
}

int findLowest(unsigned char a, unsigned char b, unsigned char c, int pos){
  if (((a<b) && (a<c)) || ((a==b) && (a==c) && (b==c))){
    return pos;
  } else if ((b < a) && (b < c)){
    return (pos+1);
  } else {
    return (pos+2);
  }
}

int encode(char phrase[50], unsigned char *bits){
  int len = strlen(phrase);
  bool a = true;
  int count = 0;
  int pos = 0;
  int *binPhrase = 0;
  for (int i = 0; i < len; i++){
    binPhrase[i] = *toBinary(strtol(&phrase[i], NULL, 10));
  } 
 // int *binPhrase = toBinary(strtol(phrase, NULL, 10));
  int *binBits = toBinary((int)*bits);
   
  while (a == true){
    if (count == len){
      return 0;
    }

    for (int i = 0; i < (strlen(phrase)*8); i++){
      if (binPhrase[i]!=binBits[i]){
        bits[i] = binPhrase[i];
      }
    }
   //   printf("%d, %d, %d\n", bits[pos], bits[pos+1], bits[pos+2]);
   // for (int i = 0; i < 3; i++){
     // int lowest = findLowest(bits[pos], bits[pos+1], bits[pos+2], pos);
     // printf("%d\n",  bits[lowest]);  
    //bits[lowest] = phrase(in bits)[count];
    //}
    pos+=3;
    count+=1;
  }

  return 0;
}

int main(int argc, char** argv) {
   struct ppm_pixel *rgb;
   int w = 0;
   int h = 0;
   int count = 0;
   char phrase[50];

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
       printf("%d, %d, %d\n", bits[count], bits[count+1], bits[count+2]);
       count+=3;
     }
     printf("\n");
   }
  
   printf("Reading %s with width %d and height %d\n", argv[1], w, h);
   printf("Max number of characters in the image: %d\n", (count/8));
   printf("Enter a phrase: ");
   scanf("%s", phrase);
   encode(phrase, bits);
  return 0;
}

