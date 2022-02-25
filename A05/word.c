#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char *word = malloc((sizeof(char)*4)+1);
  unsigned int word_int = 0;
  char *hex = malloc((sizeof(char)*8)+1);
  printf("Enter 4 characters: ");
  scanf(" %s", word);
  int j = 0;
  
  for (int i = 0; i < strlen(word); i++){
    sprintf(hex + j, "%02X", word[i]);
    word_int += hex[i]*(16^i);
    j+=2;
  } 
  printf("Your number is: %d (0x%s)\n", word_int, hex);

  free(word);
  word = NULL;
  free(hex);
  hex = NULL;
  return 0;
}
