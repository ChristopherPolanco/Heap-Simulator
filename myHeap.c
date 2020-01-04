#include "myHeap.h"
#include "myMemory.h"

#include <stdlib.h>

void v(int verb){
  makeVerbose(verb);
}

void* myMalloc(size_t size){
  return mAllocate(size);
}

void myFree(void* point){
  mFree(point);
}

void h(int size){
  sizeHeap(size);
}