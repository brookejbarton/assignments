#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int in_use;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }
  int option = 1; //0 for best fit, 1 for first fit
  struct chunk *next = flist;
  struct chunk *prev = NULL;

  if (option == 1){
    while (next!=NULL){
      if (next->size >= size){
        if (prev!=NULL){
          prev->next = next->next;
          prev->in_use = size;
        } else {
          flist = next->next;
        }
        return (void*)(next+1);
      } else {
        prev = next;
        next = next->next;
      }
    }
    void *memory = sbrk(size + sizeof(struct chunk));
    if (memory == (void *) -1) {
      return NULL;
    } else {
      struct chunk* cnk = (struct chunk*) memory;
      cnk->size = size;
      cnk->in_use = size;
      return (void*) (cnk + 1);
    }
  } else { 
    //best fit -- doesn't work
    int best_fit = 0;
    while (next!=NULL){
        if (next->size >= size && next->size <= best_fit){
          if (prev!=NULL){
            prev->next = next->next;
            prev->in_use = size;
          } else {
            flist = next->next;
            prev->in_use = size;
          }
          return (void*)(next+1);
        } else {
          best_fit = prev->size;
          prev = next;
          next = next->next;
        }
      }
      void *memory = sbrk(size + sizeof(struct chunk));
      if (memory == (void *) -1) {
        return NULL;
      } else {
        struct chunk* cnk = (struct chunk*) memory;
        cnk->size = size;
        cnk->in_use = size;
        return (void*) (cnk + 1);
      }
  }
}

void free(void *memory) {
  if (memory != NULL){
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory-1);
    cnk->next = flist;
    flist = cnk;
    flist->in_use = 0;
  }
  return;
}

void fragstats(void* buffers[], int len) {
  int total = 0;

  int in_use = 0;
  int use_size = 0;
  int l_use = 0;
  int s_use = 0;
  int a_use = 0;

  int free = 0;
  int free_size = 0;
  int l_free = 0;
  int s_free = 0;
  int a_free = 0;

  while (flist!=NULL){
    total+=1;

    if (flist->in_use!=0){
      in_use+=1;
      use_size+=flist->size;

      if (flist->size > l_use) l_use = flist->size;
      if (s_use == 0){
        s_use = flist->size;
      } else if (flist->size < s_use) s_use = flist->size;
    } else {
      free+=1;
      free_size+=flist->size;

      if (flist->size > l_free) l_free = flist->size;
      if (s_free == 0){
        s_free = flist->size;
      } else if (flist->size < s_free) s_free = flist->size;
    }

    flist = flist->next;
  }

  a_use = use_size/in_use;
  a_free = free_size/free;

  printf("Total blocks: %d, Free: %d, Used: %d\n", total, in_use, free);
  printf("Free: largest - %d, smallest - %d, average - %d\n", l_free, s_free, a_free);
  printf("In use: largest - %d, smallest - %d, average - %d\n", l_use, s_use, a_use);
}

