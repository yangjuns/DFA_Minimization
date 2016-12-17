#include <iostream>
#include <chrono>
#include <set>

using namespace std;
using namespace std::chrono;

#include "minimization.hpp"
#include "util.cpp"

DFA* generate_random_DFA(int size, int alpha_size){
  DFA* A = new DFA();
  A->size = size;
  A->alpha_size = alpha_size;
  A->inits = {1};
  /*generate random final sets*/
  set<int> finals = {};
  int n = rand() % A->size + 1;
  int i = 0;
  while(i < n){
    finals.insert(rand() % A->size +1);
    i++;
  }
  A->finals = finals;
  /* generate random transitions */
  vector<vector<int>> trans(A->alpha_size);
  for(int i =0;i < trans.size(); i++){
    trans[i] = vector<int>(A->size);
    for(int j=0; j <trans[i].size(); j++){
      trans[i][j] = rand() % A->size + 1;
    }
  }
  A->transitions = trans;
  return A;
}

void compare_runtime(int runs, int size, int alpha_size){
  int n = runs;
  int i = 0;
  int m_sum = 0;
  int b_sum = 0;
  while(i < n){
    DFA* A = generate_random_DFA(size, alpha_size);
    high_resolution_clock::time_point t1, t2;
    t1 = high_resolution_clock::now();
    Moores(A);
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    m_sum += duration;

    t1 = high_resolution_clock::now();
    Brzozowski(A);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    b_sum += duration;
    i++;
  }
  printf("Moores AVG Run Time: %d\n", m_sum/runs);
  printf("Brzozowski  AVG Run Time: %d\n", b_sum/runs);
}

int main(){
  /* Examples (Sanity check) from Lecture Minimization P.27*/
  printf("Sanity Check ....\n");
  high_resolution_clock::time_point t1, t2;
  DFA* A = new DFA();
  vector<vector<int> > trans = {{2,2,5,6,5,6},{3,4,6,5,5,6}};
  A->alpha_size = 2;
  A->size = 6;
  A->transitions = trans;
  set<int> inits = {1};
  A->inits = inits;
  set<int> finals = {3,4};
  A->finals = finals;
  t1 = high_resolution_clock::now();
  DFA* Am = Moores(A);
  t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count();
  printf("Moores Run Time: %ld\n", duration);
  printf("Output: \n");
  print_DFA_transition(Am);
  printf("Final states: ");
  print_set(Am->finals);
  printf("=========================\n");


  t1 = high_resolution_clock::now();
  DFA* Ab = Brzozowski(A);
  t2 = high_resolution_clock::now();
  duration = duration_cast<microseconds>( t2 - t1 ).count();
  printf("Brzozowski Run Time: %ld\n", duration);
  printf("Output: \n");
  print_DFA_transition(Ab);
  printf("Final states: ");
  print_set(Ab->finals);
  printf("==========================\n");

  /* Experiments */
  printf("Experiments with lots of input ...\n");
  compare_runtime(500, 10, 2);
}