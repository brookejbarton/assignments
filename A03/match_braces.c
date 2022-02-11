//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name:
//
#include <stdio.h>
#include <stdlib.h>

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
  struct node *n;
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

  return ret;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* head) {
  struct node *ret = head;
  ret->next = NULL;
  free(head);

  return ret;
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
  char getChar;
  int line = 0;
  int col = 0;
  struct node *head = NULL;
  struct node *pull = NULL;
  struct node *tmp = NULL;  

  //print usage if user input inocrrect num of command line args  
  
  if (infile == NULL){
    printf("Error: unable to open file %s\n", "prog1.c");  
  }
//TO WORK ON: HOW TO READ CHAR BY CHAR?
  while (getChar!='\0'){
    getChar = fgetc(infile);
    col+=1;
    if (getChar = '\n'){
      line+=1;
    }
    if (getChar = '{'){
      head = push(getChar, line, col, tmp);
    }
    if (getChar = '}'){
      tmp = head;
      pull = pop(head);
      if (pull->sym == tmp->sym){
        printf("Unmatched brace on Line %d and Column %d", line, col);
      }
      else {
        printf("Found matching braces: (%d, %d) -> (%d,%d)", tmp->linenum, 
              tmp->colnum, pull->linenum, pull->colnum);
      }
    }
  }
  print(head); 
  return 0;
}
