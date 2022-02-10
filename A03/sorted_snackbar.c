//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name:
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};

// Insert a new node to a list (implemented as a linked list). 
// The new node should store the given properties
// Param snacks: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list
struct snack* insert_sorted(struct snack *snacks, 
  const char *name, int quantity, float cost) {
  struct snack *n = malloc(sizeof(struct snack));
  
  if (n == NULL){
    printf("EEROR: Out of space!\n");
    exit(1);
  }
  
  strcpy(n->name, name);
  n->quantity = quantity;
  n->cost = cost;
  
  if (snacks->name[0] > n->name[0]){ //so if n comes first alphabetically
    n->next = snacks; 
  } else {
    snacks->next = n;
  }

  return snacks;
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
}

int main() {
  int n = 0;
  char name[32];
  int quantity = 0;
  float cost = 0;
  struct snack* snacks;

  printf("Enter a number of snacks: ");
  scanf("%d", &n);
  
  for (int i = 0; i < n; i++){
   // struct snack node;
    printf("\nEnter a name: ");
    scanf("%s", name);
    printf("\nEnter a cost: ");
    scanf("%f", &cost);
    printf("\nEnter a quantity: ");
    scanf("%d", &quantity);

    if (i == 0){
      snacks = insert_sorted(NULL, name, cost, quantity);
    } else {
      insert_sorted(snacks, name, cost, quantity);
    }
      
  }

  return 0;
}

