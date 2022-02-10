#include <stdio.h>
#include <string.h>
int main(){
  char word[32];
  char symbol;
  int len = 0;

  printf("Please enter a word: ");
  scanf("%s", word);
  printf("\nPlease enter a symbol: ");
  scanf(" %c", &symbol);
  
  len = strlen(word);

  for (int i = 0; i < (len+4); i++){
    printf("%c", symbol);
  }
  printf("\n%c %s %c\n", symbol, word, symbol);
  for (int i = 0; i < (len+4); i++){
    printf("%c", symbol);
  }  
 return 0; 
}
