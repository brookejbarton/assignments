/*author @bbarton
  Allows user to add snacks to a list then prints said list
  2/5/22
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

int main() {
  int n = 0;
  char name[32];
  float cost = 0;
  int quant = 0;
  
  printf("Enter a number of snacks: ");
  scanf("%d", &n);
 //use malloc instead of doing this, bc setting snackbar[n] doesn't always work 
  struct snack snackbar[n];
  
  for (int i = 0; i < n; i++){
    printf("\nEnter a name: ");
    scanf("%s", snackbar[i].name);
    printf("\nEnter a cost: ");
    scanf("%f", &snackbar[i].cost);
    printf("\nEnter a quantity: ");
    scanf("%d", &snackbar[i].quantity);
  }

  printf("\nWelcome to the Salt Lick!\n");
  
  for (int i = 0; i < n; i++){
    printf("\n%d) %s  cost: $%0.2f  quantity: %d", i, snackbar[i].name, 
           snackbar[i].cost, snackbar[i].quantity);
  }
  printf("\n");
  return 0;
}
