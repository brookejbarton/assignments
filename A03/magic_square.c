//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name: Brooke Barton
//
/** 1 3 3
  2 9 1 6
  3 3 5 7
  4 4 8 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool check_UTIL(char **matrix, int n, int magic_const){
  int checker_row = 0;
  int checker_col = 0;
  int checker_rdiag = 0;
  int checker_ldiag = 0;
  int up = 0;
  int down = (n-1); 
  bool a = false;

  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
     
      if (i == up && j == down){
        checker_ldiag = checker_ldiag + (matrix[i][j]-'0');
        a = true;
      }
      if (i == j){
        checker_rdiag = checker_rdiag + (matrix[i][j]-'0');
      }
      checker_row = checker_row + (matrix[i][j]-'0');
      checker_col = checker_col + (matrix[j][i]-'0');      
    }

    if (checker_row != magic_const || checker_col != magic_const){
      return false;
    }
    if (a == true){
      up += 1;
      down -=1;
      a = false;
    }
    checker_row = 0;
    checker_col = 0;
  }
  
  if (checker_rdiag != magic_const || checker_ldiag != magic_const){
    return false;
  }
  return true;
}

void check_magic(char **matrix, int n){
  int magic_const = 0;

  for (int i = 0; i < n; i++){
    magic_const = magic_const + (matrix[i][0]-'0');   
  }
  
  if (check_UTIL(matrix, n, magic_const) == true || n ==1){
    printf("M is a magic square (magic constant = %d)\n", magic_const);
  } else {
    printf("M is NOT a magic square!\n");
  }
}

void init_matrix(int **matrix, int n, int *input){
  int pos = 0;

  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      matrix[i][j] = input[j+pos];
    }
    pos = pos + 3;
  }

  //check_magic(matrix, n);
}

int main() {
  FILE *infile = NULL;//stdin;
  infile = fopen("magic4.txt", "r");
  char strfile_info[5];
  int n = 0;
  int pos = 0;
   
  if (infile == NULL){
    printf("Error: unable to open file %s\n", "magic1.txt");
    exit(1);
  }

  fscanf(infile, "%[^\n]", strfile_info);
  sscanf(strfile_info, "%d", &n);
  
  int *input = malloc(sizeof(int)*((n*n)+1));
  char *buffer = malloc(sizeof(char)*((n*n)+1)); 
  int **matrix; //malloc(sizeof(char) * n * n);

  char *whitespace = " \n";
  char *token; 
  
  matrix = malloc(sizeof(int *) * n);
  for (int i = 0; i < n; i++){
    matrix[i] = malloc(sizeof(int) * n);
  }

  if (input == NULL || buffer == NULL || matrix == NULL || whitespace == NULL ||
      token == NULL){
      printf("ERROR: malloc failed!\n");
      exit(1);
  }

   while (buffer != NULL){
    buffer = fgets(buffer, 1024*sizeof(char), infile);
    token = strtok(buffer, whitespace); 

  /*ISSUE IS THAT INPUT NEEDS TO BE ARR OF INT BC IF THE NUMBER IS DOUBLE
    DIGIT THEN IT COUNTS AS TWO CHARS AND U GET SEG FAULT SO I NEED TO CONVERT
    THE TOKENS TO INT AND ADD THEM TO INPUT THEN MAKE MATRIX AN INT MATRIX
    AND GO THROUGH AND EDIT ALL INT CONVERSIONS
  */
    while (token != NULL){ //only adds valuable chars into array
      input[pos] = (token -'0');  
      
      //sscanf(token, "%d", input);
      //strcat(input, token);
      token = strtok(NULL, whitespace);
      pos+=1;
    }
  } 
  fclose(infile);
  
  for (int i = 0; i < n; i++){
    printf("%d", input[i]);
  }    
  init_matrix(matrix, n, input);

//gets an error when i try to free these pointers  
//  free(buffer);
//  buffer = NULL;
//  free(token);
//  token = NULL;
//  free(whitespace);
//  whitespace = NULL;
  free(input);
  input = NULL;
  free(matrix);
  matrix = NULL;
}

