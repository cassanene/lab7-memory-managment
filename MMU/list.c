// list/list.c
// 
// Implementation for linked list.

// https://www.geeksforgeeks.org/linked-list-set-2-inserting-a-node/ --> used this for some extra guidance
// https://medium.com/better-programming/introduction-to-linked-lists-in-c-80fc426a6a2f

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* list = (list_t*)malloc(sizeof(list_t));
  list->head = NULL;
  return list; 
}

node_t *node_alloc(block_t *blk) {   
  node_t* node = malloc(sizeof(node_t));
  node->next = NULL;
  node->blk = blk;
  return node; 
}

void list_free(list_t *l){
  free(l);
  return;
}

void node_free(node_t *node){
  free(node);
  return;
}

void list_print(list_t *l) {
  node_t *current = l->head;
  block_t *b;
  
  if (current == NULL){
    printf("list is empty\n");
  }
  while (current != NULL){
    b = current->blk;
    printf("PID=%d START:%d END:%d", b->pid, b->start, b->end);
    current = current->next;
  }
}

int list_length(list_t *l) { 
  node_t *current = l->head;
  int i = 0;
  while (current != NULL){
    i++;
    current = current->next;
  }
  
  return i; 
}

void list_add_to_back(list_t *l, block_t *blk){  
  node_t* newNode = node_alloc(blk);
  newNode->next = NULL;
  if(l->head == NULL){
    l->head = newNode;
  }
  else{
    node_t *current = l->head;
    while(current->next != NULL){
      current = current->next;
    }
    current->next = newNode;
  }
}

void list_add_to_front(list_t *l, block_t *blk){  
  node_t* newNode = node_alloc(blk);
 
  newNode->next = l->head;
  l->head = newNode;
}

void list_add_at_index(list_t *l, block_t *blk, int index){
  int i = 0;
  
  node_t *newNode = node_alloc(blk);
  node_t *current = l->head;

  if(index == 0){
    newNode->next = l->head->next;
    l->head = newNode;
  }
  else if(index > 0){
    while(i < index && current->next != NULL){
      current = current->next;
      i++;
    }
  newNode->next = current->next;
  current->next = newNode;
  }
}

void list_add_ascending_by_address(list_t *l, block_t *newblk){
  
   /*
   * 1. Insert newblk into list l in ascending order based on the START address of the block.
   * 
   *    node_t *c = l.head;
   *    Insert newblk After Current Node if:   newblk->start > c->start
   */
    node_t *current;
    node_t *prev;
    node_t *node = node_alloc(newblk);

    if(l->head == NULL){ //list empty
      l->head = node;
    }
    else{
      prev = current = l->head;

      if(current->next == NULL) {  //only one node
         if(newblk->start <= current->blk->start) {  
            node->next = l->head;
            l->head = node;
         }
         else { 
            current->next = node;
            node->next = NULL;
         }
      }
      else {

         if(node->blk->start <= current->blk->start) { 
            node->next = l->head;
            l->head = node;
         }
         else {
            while(current != NULL && node->blk->start >= current->blk->start) {
                 prev = current;
                 current = current->next;    
            }
            prev->next = node;
            node->next = current;
         }
      }
    }
}

void list_add_ascending_by_blocksize(list_t *l, block_t *newblk){
   /*
   * 1. Insert newblk into list l in ascending order based on the blocksize.
   *    blocksize is calculated :  blocksize = end - start +1
   * 
   *    Ex:  blocksize = newblk->end - newblk->start
   * 
   *         node_t *c = l.head;
   * 
   *         curr_blocksize = c->blk->end - c->blk->start +1;
   * 
   *         Insert newblk After Current Node if:   blocksize >= curr_blocksize
   * 
   *    USE the compareSize()
   */
  node_t *currNode;
  node_t *prevNode;
  node_t *node = node_alloc(newblk);
  int block = node->blk->end - node->blk->start;
  int currBlock;
  
  
  if(l->head == NULL){
    l->head = node;
  }
  
  else{
    prevNode = currNode = l->head;
    
    currBlock = currNode->blk->end - currNode->blk->start + 1;
    
    if(currNode->next == NULL) {  //only one node
       if(block >= currBlock) {  
          node->next = l->head;
          l->head = node;
       }
       else {   
          currNode->next = node;
          node->next = NULL;
       }
    }
    else { // > 1 node
      
       if(block >= currBlock) {  
          node->next = l->head;
          l->head = node;
       }
       else {
      
          while(currNode != NULL && block <= currBlock) {
               prevNode = currNode;
               currNode = currNode->next;
               
               if(currNode != NULL)  
                     currBlock = currNode->blk->end - currNode->blk->start;
          }
          prevNode->next = node;
          node->next = currNode;
       }
    }
  }
}

void list_add_descending_by_blocksize(list_t *l, block_t *blk){
  node_t *current;
  node_t *prev;
  node_t *newNode = node_alloc(blk);
  int newblk_size = blk->end - blk->start;
  int curblk_size;
  
  if(l->head == NULL){
    l->head = newNode;
  }
  else{
    prev = current = l->head;
    
    curblk_size = current->blk->end - current->blk->start + 1;
    
    if(current->next == NULL) {  //only one node in list
       if(newblk_size >= curblk_size) {  // place in front of current node
          newNode->next = l->head;
          l->head = newNode;
       }
       else {   // place behind current node
          current->next = newNode;
          newNode->next = NULL;
       }
    }
    else {  // two or more nodes in list
      
       if(newblk_size >= curblk_size) {  // place in front of current node
          newNode->next = l->head;
          l->head = newNode;
       }
       else {
      
          while(current != NULL && newblk_size <= curblk_size) {
               prev = current;
               current = current->next;
               
               if(current != NULL)  // the last one in the list
                     curblk_size = current->blk->end - current->blk->start;
          }
          prev->next = newNode;
          newNode->next = current;
       }
    }
  }
}

void list_coalese_nodes(list_t *l){ 
  /*
   * 1. Assuming you have passed in a sorted list of blocks based on addresses in ascending order
   * 2. While list is not empty,
   *    a. compare two nodes at a time to see if the prev.END + 1 == current.START, if so, they are physically adjacent
   *    combine them by setting the prev.END = current.END. 
   *    b. If not adjacent go to #6
   * 3. point the prev.NEXT to the current.NEXT to skip over current.
   * 4. Free current
   * 5. go back to #2
   * 6. Advance prev = current, and current = current.NEXT
   * 7. go back to #2
   * 
   * USE the compareSize()
   */
  if (!l->head || !l->head->next){
      return;
    }
     
    // making both nodes
    node_t *prevNode = l->head;
    node_t *currNode = l->head->next;

    // while there is a node in the list
    while(currNode){
      //comparing the nodes
      if (prevNode->blk->end + 1 == currNode->blk->start){
        prevNode->blk->end = currNode->blk->end;
        prevNode->next = currNode->next;

        free(currNode->blk);
        free(currNode);

        currNode = prevNode->next;
      }
      else{
        prevNode = currNode;
        currNode = currNode->next;
      }
    }
}

block_t* list_remove_from_back(list_t *l){
  block_t *value = NULL;
  node_t *current = l->head;

  if(l->head != NULL){
    
    if(current->next == NULL) { // one node
         l->head->next = NULL;
         value = current->blk;
         node_free(current);
    }
    else {
         while (current->next->next != NULL){
            current = current->next;
         }
         value = current->blk;
         node_free(current->next);
         current->next = NULL;
    }
  }
  return value;
}

block_t* list_get_from_front(list_t *l) {
  block_t *value = NULL;
  if(l->head == NULL){
    return value;
  }
  else{
    node_t *current = l->head;
    value = current->blk;
  }
  return value; 
}


block_t* list_remove_from_front(list_t *l) { 
  block_t *value = NULL;
  if(l->head == NULL){
    return value;
  }
  else{
    node_t *current = l->head;
    value = current->blk;
    l->head = l->head->next;
    node_free(current);
  }
  return value; 
}

block_t* list_remove_at_index(list_t *l, int index) { 
  int i;
  block_t* value = NULL;
  
  bool found = false;
  
  if(l->head == NULL){
    return value;
  }
  else if (index == 0){
    return list_remove_from_front(l);
  }
  else if (index > 0){
    node_t *current = l->head;
    node_t *prev = current;
    
    i = 0;
    while(current != NULL && !found){
      if(i == index)
          found = true;
      else {
         prev = current;
         current = current->next;
         i++;
      }
    }
    
    if(found) {
      value = current->blk; 
      prev->next = current->next;
      node_free(current);
    }
  }
  return value; 
}

bool compareBlks(block_t* a, block_t *b) {
  
  if(a->pid == b->pid && a->start == b->start && a->end == b->end)
     return true;
  
  return false;
}

bool compareSize(int a, block_t *b) {  // greater or equal
  
  if(a <= (b->end - b->start) + 1)
     return true;
  
  return false;
}

bool comparePid(int a, block_t *b) {
  
  if(a == b->pid)
     return true;
  
  return false;
}


bool list_is_in(list_t *l, block_t* value) { 
  node_t *current = l->head;
  while(current != NULL){
    if(compareBlks(value, current->blk)){
      return true;
    }
    current = current->next;
  }
return false; 
}

block_t* list_get_elem_at(list_t *l, int index) { 
 int i;
  block_t* value = NULL;
  if(l->head == NULL){
    return value;
  }
  else if (index == 0){
    return list_get_from_front(l);
  }
  else if (index > 0){
    node_t *current = l->head;
    
    i = 0;
    while(current != NULL){
      if(i == index)
          return(current->blk);
      else {
         current = current->next;
         i++;
      }
    }
  }
  return value; 
}

int list_get_index_of(list_t *l, block_t* value){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  while (current != NULL){
   if (compareBlks(value,current->blk)){
     return i;
    }
    current = current->next;
    i++;
  }
  return -1; 
}

/* Checks to see if block of Size or greater exists in the list. */
bool list_is_in_by_size(list_t *l, int Size){ 
  node_t *current = l->head;
  while(current != NULL){
    if(compareSize(Size, current->blk)){
      return true;
    }
    current = current->next;
  }
return false; 
}

/* Checks to see if pid of block exists in the list. */
bool list_is_in_by_pid(list_t *l, int pid){ 
  
  // /* Iterate through the list to find a node with a blk that has blk->pid = pid
  //  * 
  //  * USE the comparePID()
  //  * 
  //  * Look at list_is_in_by_size()
  //  * /
  node_t *currNode = l->head;

  //while the current node is not null
  while(currNode != NULL){
    if(comparePid(pid, currNode->blk)){
      return true;
    }
    currNode = currNode->next;
  }
  return false;
}

/* Returns the index at which the given block of Size or greater appears. */
int list_get_index_of_by_Size(list_t *l, int Size){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  while (current != NULL){
   if (compareSize(Size,current->blk)){
     return i;
    }
    current = current->next;
    i++;
  }

  return -1; 
}
                   
/* Returns the index at which the pid appears. */
int list_get_index_of_by_Pid(list_t *l, int pid){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  while (current != NULL){
   if (comparePid(pid,current->blk)){
     return i;
    }
    current = current->next;
    i++;
  }

  return -1; 
}
