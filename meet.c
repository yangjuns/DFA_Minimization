#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

hashtable_t *table;

int* meet(int r[], int s[], int n){
  int p, val, i ,j;
  int* t = malloc(sizeof(int) * n);
  for(p = 0; p < n; p++){
    i = r[p];
    j = s[p];
    val = ht_get(table, i, j);
    if(val == -1){
      t[p] = p + 1;
      ht_set(table, i, j, p + 1);
    }else{
      t[p] = val;
    }
  }
  return t;
}

int main(){
  table = ht_create(100);
  int r[8] = {1, 1, 1, 1, 5, 5, 1, 5};
  int s[8] = {1, 2, 2, 2, 1, 1, 1, 2};
  int* t = meet(r, s, 8);
  for(int i = 0; i < 8;i++){
    printf("%d ", t[i]);
  }
  printf("\n");
  free(t);
  ht_free(table);
}