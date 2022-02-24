#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
 // char input[4];
  char *word = malloc((sizeof(char)*4)+1);
  unsigned int word_int = 0;
  char *hex = malloc((sizeof(char)*8)+1);
  printf("Enter 4 characters: ");
  scanf(" %s", word);
  int j = 0;

  //strcpy(word, input);
  
  for (int i = 0; i < strlen(word); i++){
    printf("Debug: %c = 0x%08X\n", word[i], word[i]);
    sprintf(hex + j, "%02X", word[i]);
    j+=2;
  }  
  word_int = strtol(word, '\0', 10); 
  printf("Your number is: %d (0x%s)\n", word_int, hex);
  return 0;
}
