#include "set.h"

struct q_entry_s{
  set_t* set;
  int seq;
  struct q_entry_s* next;
};

typedef struct q_entry_s q_entry_t;

struct queue_s{
  q_entry_t* head;
  q_entry_t* tail;
  int size;
};

typedef struct queue_s queue_t;

queue_t *init_queue();
void enque(queue_t * Q, set_t* set);
set_t* deque(queue_t *Q);
int is_empty(queue_t *Q);
int q_contains(queue_t *Q, set_t* set);
int q_get_seq(queue_t *Q, set_t* set);
int q_size(queue_t *Q);