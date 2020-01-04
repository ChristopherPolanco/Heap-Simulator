#define USE_REPLACEMENT_MANAGER   1
#include <stdio.h>
#if USE_REPLACEMENT_MANAGER
  #undef malloc
  #define malloc myMalloc

  #undef free
  #define free myFree

  #include "myHeap.h"
#else

  #include <stdlib.h>

#endif

//fix free so that it can take in nothing

int main(void) {
  v(1);
  h(1000);

  char* a = malloc(200);
  char* b = malloc(300);
  char* c = malloc(200);
  char* e = NULL;
  char* f = malloc(50);
  char* g = malloc(50);
  free(g);  

  return 0;  
}
