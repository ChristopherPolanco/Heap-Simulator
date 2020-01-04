#include "myMemory.h"

#include <stdlib.h>

//stddef for size_t
#include <stddef.h>
//for printf
#include <stdio.h>

int x;

void sizeHeap(int s){
  x = s;
}

//Make Node
typedef struct Node{
  size_t size;
  char* address;
  //Keep track of whether it is allocated or not
  //if node is created it is because memory has been approved for allocation
  int allocated;
  struct Node* prev;
  struct Node* next;
}Node; 

//Define heap and list
//All private
#define theHeap x
char* psuedoHeap = NULL;
Node* linkedList = NULL;
int verb = 1;


void makeVerbose(int v){
  verb = v;
}


//Starts the heap and linkedList
void startHeapList(){
  //use malloc to create psuedo heap
  psuedoHeap = (char*) malloc(theHeap);

  if(verb == 1){
    printf("INITIATING HEAP\n\n");
  }

//start linked list
//start node is the entirety of allocated memory to heap
  Node* head = (Node*) calloc(1, sizeof(Node));

  //start address is heap because we need to keep track of the memory
  head->address = psuedoHeap;

  //size is theHeap because that is the size of pseudoHeap
  head->size = theHeap;

  //since the head represents the heap allocated has to be 0
  head->allocated = 0;

  //set linkedList = to start
  //so whenever heap is initiated so is list
  linkedList = head;
}




//function hole finds a hole in the hopes to allocate the requested size 
//if it can not find a hole that is big enough it returns null
char* hole(size_t s){
  Node* check = linkedList;

  while(check != NULL){
    //here we see if it is a hole and greater than requested size
    if(check->allocated == 0 && check->size > s){
      //if true, subtract check's size due to s allocation
      check->size = check->size - s;

      //make node
      Node* node = (Node*) calloc(1, sizeof(Node));

      //set node attributes equal to correct values
      node->size = s;

      node->next = check->next;
      check->next = node;
      node->prev = check;

      node->allocated = 1;

      node->address = (check->address + check->size) - node->size;
      
      if(verb == 1){
        printf("Allocated : %zu", s);
        printf("\n");
      }

      return node->address;
    }

    //here we see if it is a hole and equal to the requested size
    else if(check->allocated == 0 && check->size == s){
      check->allocated = 1;

      if(verb == 1){
        printf("Allocated : %zu", s);
        printf("\n");
      }


      return check->address;
    }
    check = check->next;
  }

 //if unable to find a hole return null
  printf("Can not allocate: %zu\n",s); 
  return NULL;
}

//checkFit's purpose is to see if it can be allocated and added to the list
//also looks for hole that can fit requested size
//returns NULL if it can't
char* checkFit(size_t s){
  Node* check = linkedList;
  //check if the heap on its own has memory
  if(check->allocated != 1){
    //check if the size can accomodate the request
    if(check->size > s){
      //if true, subtract check's size due to s allocation
      check->size = check->size - s;

      //make node
      Node* node = (Node*) calloc(1, sizeof(Node));

      //set node attributes equal to correct values
      node->size = s;

      node->next = check->next;
      check->next = node;
      node->prev = check;

      node->allocated = 1;

      node->address = (check->address + check->size) - node->size;

      if(verb == 1){
        printf("Allocated : %zu", s);
        printf("\t\t");
        printf("Remaining Heap: %zu", linkedList->size);
        printf("\n");
      }

      return node->address;
    }

    else if(check->size == s){
      //if requested equals the amount of memory left then
      Node* node = (Node*) calloc(1, sizeof(Node));

      node->next = check->next;
      check->next = node;
      node->prev = check;

      node->size = s;
      node->allocated = 1;
      node->address = check->address;


      check->size = 0;

      if(verb == 1){
        printf("Allocated : %zu", s);
        printf("\t\t");
        printf("Remaining Heap: %zu", linkedList->size);
        printf("\n");
      }


      return check->address;
    }
  }    

  //if we reach this point it means first node is allocated or
  //the requested memory does not fit in first node space which means we need to find a hole
  char* result = hole(s);
  return result;
}

void* mAllocate(size_t s){
  //create psuedo heap of size theHeap
  if(psuedoHeap == NULL){
    startHeapList();
  }

  char* result = checkFit(s);
  return result;
}


void printList(){
  Node* check = linkedList;

  printf("Remaining heap size is: %zu\n", check->size);
  check = check->next;

  while(check != NULL){
    if(check->size != 0){
      printf("Allocated: ");
      printf("%zu\n", check->size);
    }
    check = check->next;
  }
}


//leftUnion combines two adjacent holes keep the left and free the right
void leftUnion(Node* keep, Node* leave){
  //make sure next of leave is not null
  if(leave->next != NULL){
    Node* hold = leave->next;
    hold->prev = keep;
    keep->next = leave->next;
  }

  keep->size = keep->size + leave->size;
  leave->size = 0;

  free(leave);
}

//rightUnion combines two adjacent holes keep the right and free the left
void rightUnion(Node* keep, Node* leave){

  if(leave->prev != NULL){
    Node* hold = leave->prev;
    hold->next = keep;
    keep->prev = leave->prev;
  }

  keep->size = keep->size + leave->size;
  leave->size = 0;

  free(leave);
}


//mFree takes in the start adress of the node to free
void mFree(void* p){
  //check if p is null
  if(p == NULL){
    printf("Requested node not in list\n");
    exit(0);
  }


//initialize check and point
  Node* check = linkedList;
  Node* point = NULL;


  //find node with the address of p
  while(check != NULL){
    if(check->address == p){
      point = check;
    }
    check = check->next;
  }

  //if point stays null it is not in list
  if(point == NULL){
    printf("Requested node not in list\n");
    exit(0);
  }

  size_t s = point->size;

  //make a pointer to node before point
  Node* checkPrev = point->prev;

//make sure it is not null, is a hole, and not the head
  if(point->prev != NULL && checkPrev->allocated == 0 && checkPrev != linkedList){
    //left union is to union two holes and free the desired node
    leftUnion(checkPrev, point);
    return;
  }

//make a pointer to node after point
  Node* checkNext = point->next;

//make sure point next is not null and is a hole
  if(point->next != NULL && checkNext->allocated == 0){
    //right union is to union two hole and free the desired node
    rightUnion(checkNext, point);
    return;
  }

  //if there are no adjacent holes to desired free 
  //return size to first node and free desired node
  Node* head = linkedList;
  head->size = head->size + point->size;

  free(point);

  if(verb == 1){
    printf("\nREQUESTED FREE OF: %zu", s);
    printf("\n\nAFTER REQUESTED FREE\n");
    printList();
  }
}

void mFreeAll(){
  Node* check = linkedList;
  Node* hold;
  while(check != NULL){
    if(check->next != NULL){
      hold = check->next;
    }
    mFree(check);
    check = hold;
  }
}

