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
  
/*at n >= 516 returns [double free or corruption (!prev)] error, i really
  can't figure out why but I didn't have this problem on the previous version
  of this program 
*/

  if (ret_str == NULL){
    printf("Cannot allocate new string. Exiting...\n");
    exit(1);
  }
  
  strcpy(ret_str, input);
  for (int i = 0; i < total; i++){
    strcat(ret_str, input);
  }
 
  printf("You word is "); 
  for (int i = 0; i < total; i ++){
   printf("%c", ret_str[i]);
  }
  printf("\n");

  free(ret_str);
  ret_str = NULL;
  return 0;
}
