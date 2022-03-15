/*author @bbarton
  1/28/22
  Takes an inputted word and replaces 'e' with '3', 'l' with '1', and 'a' with
  '@'
*/
#include <stdio.h>
#include <string.h>

void print(char input[]){
   int n = strlen(input);
   printf("Your bad password is ");
   for (int i = 0; i < n; i++){
     printf("%c", input[i]);
   }
  printf("\n");
}

void badification(char input[]){
	int n = strlen(input);
	for (int i = 0; i < n; i++){
		if (input[i] == 'e'){
			input[i] = '3';
		}
		if (input[i] == 'l'){
			input[i] = '1';
		}
		if (input[i] == 'a'){
			input[i] = '@';
		}
	}
	print(input);
}

int main() {
  char input[100];
  printf("Enter a word: ");
  scanf("%s", input);
  badification(input);
  return 0;
}
