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
    sprintf(hex + j, "%02x", word[i]);
    printf("char %c: %02x\n", word[i], word[i]);
    j+=2;
  } 

  word_int = strtol(hex, NULL, 16);
  printf("Your number is: %d (0x%s)\n", word_int, hex);

  free(word);
  word = NULL;
  free(hex);
  hex = NULL;
  return 0;
}
