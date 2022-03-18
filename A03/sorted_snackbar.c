//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name:
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};

void clear(struct snack* snacks) {
  struct snack* tmp = malloc(sizeof(struct snack));

  if (tmp == NULL){
    printf("ERROR: out of space!\n");
  }

  while (snacks != NULL){
    tmp = snacks;
    snacks = snacks->next;
    if (tmp != NULL){
      free(tmp);
    }
  }

  tmp = NULL;
}

void show_list(struct snack *head){
  struct snack *tmp = malloc(sizeof(struct snack));
  printf("\nWelcome to Sorted Sally's Snack Bar!\n\n");
  int i = 0;
  while (head != NULL){ 
    printf("%d) %s  cost: $%0.2f  quantity: %d\n", i, head->name, head->cost,
           head->quantity);
    tmp = head->next;
    head = tmp;
    i+=1;
  }
  free(tmp);
  tmp = NULL;
}

void get_comp_UTIL(char nameh[32], char namen[32], char *comph, char *compn){
  int total = 0;
  
  if (strlen(nameh) > strlen(namen)){
    total = strlen(nameh);
  }else {
    total = strlen(namen);
  }
  for (int i = 0; i < total; i++){
    if (nameh[i]!=namen[i]){
      *comph = nameh[i];
      *compn = namen[i];
      break;
    }
  }
}

void get_next_comp_char(struct snack *head, struct snack *n, char *comp_head_next, char *comp_n2){
  if (head->next != 0){
    if (head->next->name[0] == n->name[0]){
      get_comp_UTIL(head->next->name, n->name, comp_head_next, comp_n2);
    }else{
      *comp_head_next = head->next->name[0];
      *comp_n2 = n->name[0];
    }
  }
}

void get_comp_char(struct snack *head, struct snack *n, char *comp_head, char *comp_n){ 
  if (head->name[0] == n->name[0]){
      get_comp_UTIL(head->name, n->name, comp_head, comp_n);
  }else {
     *comp_head = head->name[0];
     *comp_n = n->name[0];
  }
}

struct snack* organize(struct snack *head, struct snack *n, struct snack *tmp,
       struct snack *ret){
 
  ret = head;
  char comp_head;
  char comp_head_next;
  char comp_n;
  char comp_n2;

  get_comp_char(head, n, &comp_head, &comp_n);
  get_next_comp_char(head, n, &comp_head_next, &comp_n2);

  while (head != NULL){    
    if (comp_head > comp_n){//if n comes first
      n->next = head;
      return n;
    }else if (comp_head < comp_n && head->next == NULL){//if n at end
      head->next = n;
      n->next = NULL;
      return ret;
    }else if (head->next != NULL){  
      if (comp_head_next > comp_n2){//if n somewhere in middle
       tmp = head->next;
       head->next = n;
       n->next = tmp;
       return ret;
      }
    }
    
    tmp = head->next;
    head = tmp;    
    get_comp_char(head, n, &comp_head, &comp_n);
    get_next_comp_char(head, n, &comp_head_next, &comp_n2);
  }


  return ret;
}

struct snack* insert_sorted(struct snack *head, char name[32], int quantity, 
       float cost, struct snack *to_return) {
  struct snack *n = malloc(sizeof(struct snack));
  struct snack *tmp = malloc(sizeof(struct snack));
  struct snack *ret = malloc(sizeof(struct snack));
  
  if (n == NULL || tmp == NULL || ret == NULL){
    printf("ERROR: Out of space!\n");
    exit(1);
  }
  
   strcpy(tmp->name,"holder");
   tmp->cost = 0;
   tmp->next = NULL;

   strcpy(ret->name, "ret");
   ret->cost = 0;
   ret->next = NULL; 
 
  strcpy(n->name, name);
  n->quantity = quantity;
  n->cost = cost;
  n->next = NULL;

  if (head == NULL){
    return n;
  }

  to_return = organize(head, n, tmp, ret);
 
  free(tmp);
  free(n);
  free(ret);
  tmp = NULL;
  n = NULL;
  ret = NULL;
  return to_return;
}

int main() {
  int n = 0;
  char name[32];
  int quantity = 0;
  float cost = 0;
  struct snack *head = malloc(sizeof(struct snack));
  struct snack *tmp = malloc(sizeof(struct snack));
  struct snack *ret = malloc(sizeof(struct snack));

  if (head == NULL || tmp == NULL || ret == NULL){
    printf("ERROR: out of space!\n");
    exit(1);
  }

  memset(ret, 0, 0);
  
  printf("Enter a number of snacks: ");
  scanf("%d", &n);
  
  for (int i = 0; i < n; i++){
    printf("\nEnter a name: ");
    scanf("%s", name);
    printf("\nEnter a cost: ");
    scanf("%f", &cost);
    printf("\nEnter a quantity: ");
    scanf("%d", &quantity);

    if (i == 0){
      head = insert_sorted(NULL, name, cost, quantity, ret);
    } else {
      tmp = insert_sorted(head, name, cost, quantity, ret);
      head = tmp;
    }  
  }

 show_list(head);
 free(tmp);
 free(head);
 free(ret);
 tmp = NULL;
 head = NULL;
 ret = NULL;
  return 0;
}

