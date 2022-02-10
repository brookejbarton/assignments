/*author @bbarton
  Randomly chooses a word and presents a hangman style game
  2/4/22
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

void make_stage(char *stage, size_t length){
  for (int i = 0; i < length; i++){
    printf("%c ", stage[i]);
  }
  printf("\n");
}

bool check_word(char *stage, char word[32], char input, int *tracker);

int start_game(char *stage, char word[32], int *tracker, int *turns, 
 char *input){
  if ((size_t)*tracker >= strlen(word)){
    printf("\nYou won in %d turns!\n", (*turns-1));
    return 0;
  }

  printf("Guess a character: ");
  scanf(" %c", input); //include whitespace b4 % otherwise will skip
  printf("\nTurn: %d\n", *turns);

  if (check_word(stage, word, *input, tracker) == true){
    make_stage(stage, strlen(word));
  } else {
    printf("Sorry, '%c' was not found!\n", *input);
    make_stage(stage, strlen(word));
  }
  *turns = *turns + 1;
  return start_game(stage, word, tracker, turns, input);
}

bool check_word(char *stage, char word[32], char input, int *tracker){
  bool a = false;
   for (int i = 0; i < strlen(word); i++){
      if (word[i]==input){
        stage[i] = input;
        *tracker = *tracker + 1;
        a = true;
      }
    }
    return a;
}

void start_stage(char *stage, size_t length){
  for (int i = 0; i < length; i++){
    stage[i] = '_';
  }
  stage[length+1] = '\0';
  make_stage(stage, length);
}

int main() {
  srand(time(0));
  FILE *infile = NULL;
  char lines[10];
  int n, size, pos;
  int r = 0;
  int turns = 1;
  int tracker = 0;
  char chosen_word[32];
  char *stage = NULL;
  char *inputted_letter = NULL;
  char input;
 
  infile = fopen("words.txt", "r");
  if (infile == NULL){
    printf("Error: unable to open file %s\n", "input.txt");
    exit(1);
  }
  
  fscanf(infile, "%[^\n]", lines);
  sscanf(lines, "%d", &n);

  while (r == 0||r == 1){
    r = rand() % (n+2); //+1 because the first row only has num of words
  }

  char buffer[1024];
  while (fgets(buffer, 1024, infile) != NULL){
    pos = pos + 1;
    char word[512];
    sscanf(buffer, "%s", word);
    if (pos == r){
      strcpy(chosen_word, word);
      break;
    }
  }
  fclose(infile);
  infile = NULL; //SHOULD DO THIS HERE, fclose() == free()
    
  size = strlen(chosen_word);
  stage = malloc(sizeof(char)*(size+1));
  inputted_letter = malloc(sizeof(char)*1); 
  
  if (stage == NULL||inputted_letter == NULL){
    printf("ERROR: malloc failed!\n");
    exit(1);
  } 
  strcpy(stage, chosen_word);
  
  printf("Welcome to Word Guess!\n");
  printf("\nTurn: %d\n", turns);
  turns = turns + 1;
  
  start_stage(stage, strlen(chosen_word));
  start_game(stage, chosen_word, &tracker, &turns, inputted_letter);
  
  free(stage);
  free(inputted_letter);
 // free(infile); DOING THIS CALLS A DOUBLE FREE
  stage = NULL;
  inputted_letter = NULL;
  //infile = NULL;
  
  return 0;
}
