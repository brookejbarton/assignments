/*author @bbarton
  Given an inputted string, repeats an inputted number of times
  2/5/22
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char input[32];
  char *ret_str;
  int n = 0;
  int total = 0;

  printf("Enter a word: ");
  scanf("%s", input);
  printf("\nEnter a count: ");
  scanf("%d", &n);

  total = (strlen(input)*n);
  ret_str = malloc(sizeof(char)*(total+1));  

  if (ret_str == NULL){
    printf("Cannot allocate new string. Exiting...\n");
    exit(1);
  }

  strcpy(ret_str, input);
  for (int i = 0; i < (n-1); i++){
    strcat(ret_str, input);
  }
 
  printf("You word is %s", ret_str); 
  printf("\n");

  free(ret_str);
  ret_str = NULL;
  return 0;
}
