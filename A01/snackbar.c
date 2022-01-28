/*author @bbarton
  1/28/22
  Gives a list of snacks, which you are free to purchase.
*/

#include <stdio.h>
#include <string.h>

struct snack {
	char name[15];
	float cost;
	int quantity;
};

void cashRegister(float money, struct snack choice);

int main() {
	float money;
	int choice;

	struct snack tostitos;
	strcpy(tostitos.name, "Tostitos");
	tostitos.cost = 3.49;
	tostitos.quantity = 4;
 
  struct snack persimmons;
  strcpy(persimmons.name, "Persimmons");
  persimmons.cost = 1.00;
  persimmons.quantity = 6;

  struct snack gummyShark;
  strcpy(gummyShark.name, "Gummy Sharks");
  gummyShark.cost = 2.75;
  gummyShark.quantity = 0;

	printf("Welcome to the Crust Bucket! \n"); //the name is gross, sorry
	printf("How much money do you have? ");
	scanf("%f", &money);
	printf("0) Tostitos     cost: $3.49   quantity: 4 \n");
  printf("1) Persimmons   cost: $1.00   quantity: 6 \n");
  printf("2) Gummy Sharks cost: $2.75   quantity: 0 \n");
	printf("What snack would you like to buy? [0,1,2] ");
	scanf("%d", &choice);
	if (choice == 0){
		cashRegister(money, tostitos);
	} else if (choice == 1){
    cashRegister(money, persimmons);
  } else if (choice == 2){
    cashRegister(money, gummyShark);
  } else {
    printf("That item is not sold here, sorry.\n");
  }

	return 0;
}

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
