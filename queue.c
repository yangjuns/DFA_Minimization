#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "set.h"

queue_t *init_queue(){
  queue_t *Q =malloc(sizeof(queue_t));
  Q->head = NULL;
  Q->tail = NULL;
  Q->size = 0;
  return Q;
}

void enque(queue_t * Q, set_t* set){
  q_entry_t* new_entry = malloc(sizeof(q_entry_t));
  new_entry->set = set;
  new_entry->next = NULL;
  new_entry->seq = Q->size + 1;
  if(Q->head){
    Q->tail->next = new_entry;
    Q->tail = new_entry;
  }else{
    Q->head = new_entry;
    Q->tail = new_entry;
  }
  Q->size++;
}

set_t* deque(queue_t *Q){
  set_t* result;
  if(Q->head){
    result = Q->head->set;
    if(Q->tail == Q->head)
      Q->tail = Q->tail->next;
    Q->head = Q->head->next;
    return result;
  }else{
    return NULL;
  }
  Q->size--;
}

int is_empty(queue_t *Q){
  return Q->size == 0;
}

int q_size(queue_t *Q){
  return Q->size;
}

int q_contains(queue_t *Q, set_t* set){
  q_entry_t*
}

int main(){
  queue_t* Q = init_queue();
  set_t* a = malloc(sizeof(set_t));
  int* set = malloc(sizeof(int)*2);
  set[0]=1;
  set[1]=2;
  a->set = set;
  a->size = 2;
  printf("%d\n", is_empty(Q));
  enque(Q, a);
  printf("%d\n", is_empty(Q));
  deque(Q);
  printf("%d\n", is_empty(Q));
}