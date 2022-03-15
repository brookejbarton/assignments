/*author @bbarton
  1/28/22
  Gives a list of snacks, which you are free to purchase.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct snack {
	char name[15];
	float cost;
	int quantity;
};

void cashRegister(float money, struct snack choice){
  float change;
  change = money - choice.cost;

  if (choice.quantity == 0){
    printf("Sorry, we are out of %s!\n", choice.name);
   } else if (change < 0){
    printf("You don't have enough money, sorry. \n");
  } else {
    printf("You bought %s", choice.name);
    printf("\nYou have $%0.2f left\n", change);
  }
}

int main() {
	float money;
	int choice;

  struct snack *snackbar = malloc(sizeof(struct snack)*3);
  
  if (snackbar == NULL){
    printf("ERROR: malloc failed!");
    exit(1);
  }
  
	strcpy(snackbar[0].name, "Tostitos");
	snackbar[0].cost = 3.49;
	snackbar[0].quantity = 4;
 
  strcpy(snackbar[1].name, "Persimmons");
  snackbar[1].cost = 1.00;
  snackbar[1].quantity = 6;

  strcpy(snackbar[2].name, "Gummy Sharks");
  snackbar[2].cost = 2.75;
  snackbar[2].quantity = 0;

	printf("Welcome to the Crust Bucket! \n"); //the name is gross, sorry
	printf("How much money do you have? ");
	scanf("%f", &money);
	printf("0) Tostitos     cost: $3.49   quantity: 4 \n");
  printf("1) Persimmons   cost: $1.00   quantity: 6 \n");
  printf("2) Gummy Sharks cost: $2.75   quantity: 0 \n");
	printf("What snack would you like to buy? [0,1,2] ");
	scanf("%d", &choice);
	if (choice == 0){
		cashRegister(money, snackbar[0]);
	} else if (choice == 1){
    cashRegister(money, snackbar[1]);
  } else if (choice == 2){
    cashRegister(money, snackbar[2]);
  } else {
    printf("That item is not sold here, sorry.\n");
  }

  free(snackbar);
  snackbar = NULL;
	return 0;
}
