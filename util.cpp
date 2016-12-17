#include <vector>
#include <set>
#include <stdlib.h>
#include <stdio.h>

#include "finite_machine.hpp"
#include "util.hpp"
using std::vector;
using std::set;

/* helper functions */
void print_vector(vector<int> arr){
  for(int i = 0; i < arr.size();i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void print_set(set<int> s){
  printf("{");
  for(auto it = s.begin(); it != s.end(); it++) {
    printf("%d, ", *it);
  }
  printf("}\n");
}

void print_NFA_transition(NFA* A){
  vector<vector<vector<int> > > T = A->transitions;
  for(int i =0; i < A->alpha_size; i++){
    printf("-------------%d---------\n", i);
    vector<vector<int> > t = T[i];
    for(int j =0; j< A->size; j++){
      vector<int> p = t[j];
      printf("state %d : ", j+1);
      for(int k=0; k<p.size(); k++){
        printf(" %d, ", p[k]);
      }
      printf("\n");
    }
  }
}

void print_DFA_transition(DFA* A){
  vector<vector<int> > T = A->transitions;
  printf("    ");
  for(int i = 0; i< A->size; i++){
    printf("%d ",i+1);
  }
  printf("\n----------------------\n");
  for(int i =0; i < A->alpha_size; i++){
    printf("%d : ", i);
    vector<int> t = T[i];
    for(int j =0; j< A->size; j++){
      int p = t[j];
      printf("%d ", p);
    }
    printf("\n");
  }
}