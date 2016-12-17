#ifndef SET_H
#define SET_H

struct set_s{
  int* set;
  int size;
  int capacity;
};

typedef struct set_s set_t;

set_t* set_init(int* arr, n);
void set_add(set_t* set, int elem);
int set_intersect_empty(set_t* set1, set_t* set2);


#endif