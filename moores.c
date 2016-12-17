#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"



int* meet(int r[], int s[], int n){
  hashtable_t *table = ht_create(n);
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
  ht_free(table);
  return t;
}

void print_arr(int* arr, int n){
  for(int i = 0; i < n;i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}
int min(int* arr, int n){
  int min = arr[0];
  for(int i =0 ;i < n; i++){
    if(arr[i]<min) min = arr[i];
  }
  return min;
}

int* compute_rho_f(int f[], int rho[], int n){
  int* result = malloc(sizeof(int) * n);
  for(int i =0;i <n;i++){
    result[i] = rho[f[i]-1];
  }
  return result;
}

int compare_arr(int arr1[], int arr2[], int n){
  for(int i =0; i< n; i++){
    if(arr1[i] != arr2[i]) return 0;
  }
  return 1;
}

/*
 * @param n: the number of final states
 */
int* Moores(int size, int matrix[2][size], int finals[], int n){
  /* initialize E0 */
  int F = min(finals, n);
  int non_F = F == 1? 2 : 1;
  int* E0 = malloc(sizeof(int) * size);
  for(int i =0; i< size; i++){
    /*check if it is a final state*/
    int is_final = 0;
    for(int j = 0; j< n;j ++){
      if(finals[j] == i+1){
        is_final = 1;
        break;
      }
    }
    E0[i] = is_final? F: non_F;
  }
  /* compute rho_a rho_b */
  int* E_next = E0;
  do{
    printf("hhh\n");
    E0 = E_next;
    printf("E_pre: ");
    print_arr(E0, size);
    int* rho_a = compute_rho_f(matrix[0], E0, size);
    printf("rho_a: ");
    print_arr(rho_a, size);
    int* rho_b = compute_rho_f(matrix[1], E0, size);
    printf("rho_b: ");
    print_arr(rho_b, size);
    // int* R_a = meet(rho_a, E0, size);
    // int* R_b = meet(rho_b, E0, size);
    int* temp = meet(rho_a, rho_b, size);
    E_next = meet(temp, E0, size);
    printf("E_nxt: ");
    print_arr(E_next, size);
  }while(!compare_arr(E0, E_next, size));
  return E_next;
}

int main(){
  int r[6] = {1,1,1,1,1,1};
  int s[6] = {3,3,1,1,1,1};
  int e[6] = {3,4,1,2,4,5};
  int* t1 = meet(e, e, 6);
  print_arr(t1, 6);
  int* t2  = meet(s, e, 6);
  print_arr(t2, 6);
  int* t  = meet(t1, t2, 6);
  print_arr(t, 6);

  int matrix[2][8] = {{2,4,5,2,6,8,4,6}, {3,5,4,3,7,4,8,7}};
  int finals[] = {1,4};
  int* answer = Moores(8, matrix, finals, 2);
  printf("answer:\n");
  print_arr(answer, 8);
  /* Need to free things*/
}