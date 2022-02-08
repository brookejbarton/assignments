//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name: Brooke Barton
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *infile = NULL;
  infile = fopen("magic2.txt", "r");
  char strfile_info[5];
  int n = 0;
   
  if (infile == NULL){
    printf("Error: unable to open file %s\n", "magic1.txt");
    exit(1);
  }

  fscanf(infile, "%[^\n]", strfile_info);
  sscanf(strfile_info, "%d", &n);
  
  char *input = malloc(sizeof(char)*((n*n)+1));
  char *buffer = malloc(sizeof(char)*((n*n)+1));
  char *matrix = malloc(sizeof(char) * n * n);
 
  char *whitespace = " \n";
  char *token; 
 
  while (buffer != NULL){
    buffer = fgets(buffer, 1024*sizeof(char), infile);
    token = strtok(buffer, whitespace); 
  
    while (token != NULL){ //only adds valuable chars into array
      strcat(input, token);
      token = strtok(NULL, whitespace);
    }
  } 
  fclose(infile);

  free(buffer);
  free(token);
  free(whitespace);
  buffer = NULL;
  token = NULL;   
  whitespace = NULL;  
  
  //TO DO NEXT: convert array into 2D array
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      matrix[i*n + j] = input[i];
    }
  }
  
}

