/*author @bbarton
  1/28/22
  Play rock, paper, scissors with an AI
*/

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void scoreInc(int* passScore){ //taken from the lecture
	int local = *passScore;
	local = local + 1;
	*passScore = local;
}

void moveDescript(int player, bool won){
  if (player == 0 && won == true || player == 1 && won == false){
    printf("Rock bashes scissors \n");
  } else if (player == 1 && won == true || player == 2 && won == false){
    printf("Scissors cuts paper \n");
  } else {
    printf("Paper covers rock \n");
  }
}

void outcome(int player, int outcome, int* playerScore, int* computerScore ){
   if (outcome == 0){
     printf("AI score: %d, Player score: %d \n", *computerScore, *playerScore);
   } else if (outcome == 1){
     scoreInc(computerScore);
     moveDescript(player, false);
     printf("AI score: %d, Player score: %d \n", *computerScore, *playerScore);
   } else if (outcome == 2) {
     scoreInc(playerScore);
     moveDescript(player, true);
     printf("AI score: %d, Player score: %d \n", *computerScore, *playerScore);
   }
 }

int battle(int player, int computer){
    if (player == 0 && computer == 2 || player == 1 && computer == 0
       || player == 2 && computer == 1){
     return 1; //AI wins
   } else if (computer == 0 && player == 2 || computer == 1 && player == 0
       || computer == 2 && player == 1){
     return 2; //player wins
   }
   return 0; //tie
}

void game(char input[], int opp, int* playerScore, int* computerScore) {
   int rockComp, scissorComp, paperComp, player;
   rockComp = strcmp("rock",input);
   scissorComp = strcmp("scissors",input);
   paperComp = strcmp("paper",input);
 
   if (rockComp == 0){ //rock == 0
     player = 0;
   } else if (scissorComp == 0){ //scissors == 1
     player = 1;
   } else if (paperComp == 0){ //paper == 2
     player = 2;
   } else {
     printf("You entered an invalid choice: %s\n", input);
   }
 
   if (player == 0 || player == 1 || player == 2){
     outcome(player, battle(player, opp), playerScore, computerScore);
   }
 }

int main() {
   char input[10];
   int opp = 0;
   int rounds = 0;
   int playerScore = 0;
   int computerScore = 0;
   srand(time(0));
 
   printf("Welcome to Rock, Paper, Scissors!\n");
   printf("How many rounds would you like to play? ");
   scanf("%d", &rounds);
 
   for (int i = 0; i < rounds; i++){
     printf("\nWhich do you choose? rock, paper, or scissors? ");
     scanf("%s", input);
 
     opp = rand() % 3;
     printf("AI chose ");
     if (opp == 0){
       printf("rock \n");
     } else if (opp == 1){
       printf("scissors \n");
     } else {
       printf("paper \n");
     }

     game(input, opp, &playerScore, &computerScore);
 
     if (i == (rounds-1)){
       if (playerScore > computerScore){
           printf("You won!! \n");
       } else if (computerScore > playerScore) {
           printf("You lost :( \n");
       } else {
           printf("Y'all tied. \n");
       }
     }
 }
   return 0;
 }
