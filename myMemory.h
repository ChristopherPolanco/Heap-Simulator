#ifndef myMemory
#define myMemory
#include <stddef.h>

void startHeapList();

void* mAllocate(size_t size);

void mFree(void* point);

void makeVerbose(int v);

void sizeHeap(int s);


#endif