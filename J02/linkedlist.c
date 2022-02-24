#include <stdio.h>
#include <string.h>

struct cake {
  float cost;
  char flavor[16];
  struct cake* next;
};

void cheapestCake(struct cake* cakestack){
  char cheapest[16] = "\0";
  float min = 10;

  while(cakestack!=NULL){
    printf("cake: %s, ($%0.2f)\n", cakestack->flavor, cakestack->cost);
    if (min > cakestack->cost){
      min = cakestack->cost;
      strcpy(cheapest, cakestack->flavor);
    }

    cakestack=cakestack->next;
  }

  printf("The cheapest cake is %s\n", cheapest);
}

struct cake makeCake(const char* flavor, float cost) {
  struct cake newCake;
  strncpy(newCake.flavor, flavor, 16);
  newCake.cost = cost;
  newCake.next = NULL;
  return newCake;
}

// todo: implement cheapestCake

int main() {
  struct cake cake1 = makeCake("red velvet", 2.00);
  struct cake cake2 = makeCake("chocolate", 1.75);
  struct cake cake3 = makeCake("mocha", 3.50);
  struct cake cake4 = makeCake("banana", 5.67);
  // todo: define 4th cake

  cake1.next = &cake2;
  cake2.next = &cake3;
  cake3.next = &cake4;
  // draw stack and heap here
  cheapestCake(&cake1);
  // todo: call cheapestCake
}
