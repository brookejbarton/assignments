#include <stdio.h>
#include <stdlib.h>

int main() {
  char bits[8];
  
  printf("Enter 8 bits: ");
  scanf(" %s", bits);
  char final_char = strtol(bits, (char **)NULL, 2);

  printf("Your character is: %c\n", final_char);
  return 0;
}
