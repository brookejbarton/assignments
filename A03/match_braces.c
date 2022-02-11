//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name:
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* head) {
  struct node *n = malloc(sizeof(struct node));
  struct node *ret = head;
  
  n->sym = sym;
  n->linenum = line;
  n->colnum = col;
  n->next = NULL;
  
  if (head == NULL){
    return n;
  } else {
    while (head->next != NULL){
      head = head->next;    
    }
    head->next = n;
  }

  if (ret==NULL){
    return n;
  }
  return ret;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* head) {
  if (head == NULL){
    return NULL;
  }
  if (head->next == NULL){
    return head;
  }
  struct node *ret = head;
  head = head->next;
  free(ret); //POPS HEAD SO I CAN'T DO COMP WHEN RETURNED
  ret = NULL;

  return head;
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* head) {
  struct node *tmp;
  
  while (head != NULL){
    tmp = head;
    head = head->next;
    free(tmp);
  }  
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* head) {
  while (head != NULL){
    printf("%c, %d, %d\n", head->sym, head->linenum, head->colnum);
    head = head->next;
  }
}

int main(int argc, char* argv[]) {
  FILE *infile = NULL; //stdin;
  infile = fopen("prog1.c", "r");
  char getChar = -1;
  int line = 0;
  int col = 0;
  struct node *head = malloc(sizeof(struct node));
  struct node *pull = malloc(sizeof(struct node));
  struct node *tmp = malloc(sizeof(struct node));  
  int xloc = 0;
  int yloc = 0;
   
  memset(tmp, 0, sizeof(struct node));
  //print usage if user input inocrrect num of command line args  
  
  if (infile == NULL){
    printf("Error: unable to open file %s\n", "prog1.c");  
  }

  while (getChar != '\0'){
    getChar = fgetc(infile);
    col+=1;
    if (getChar == '\n'){
      line+=1;
      col = 0;
    }
    if (getChar == '{'){
      head = push(getChar, line, col, tmp);
      tmp = head;
    }
    if (getChar == '}'){
      if (head != NULL){
       // xloc = head->linenum; //INVALID READ OF SIZE 4
       // yloc = head->colnum; //so what's happening is head gets freed later so these become invalid
      }

      pull = pop(head);
      if (pull != NULL){
        printf("Found matching braces: (%d, %d) -> (%d, %d)\n", (pull->linenum+1), 
                pull->colnum, xloc, yloc);
        if (pull->next == NULL){
          pull = NULL;
        }
      }
      else { 
        printf("Unmatched brace on Line %d and Column %d\n", (line+1), col);
      }
      if (pull == NULL){ 
        head = NULL;
        tmp = NULL;
      } else {
        head = pull;
        tmp = head;
      }
    }
  } 
  return 0;
}
