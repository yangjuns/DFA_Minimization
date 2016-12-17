#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#include "minimization.hpp"
#include "util.cpp"

int main(){
  /* Examples (Sanity check) from Lecture Minimization P.27*/
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

  t1 = high_resolution_clock::now();
  DFA* Ab = Brzozowski(A);
  t2 = high_resolution_clock::now();
  duration = duration_cast<microseconds>( t2 - t1 ).count();
  printf("Brzozowski Run Time: %ld\n", duration);

  /**/
  print_DFA_transition(Am);
  print_set(Am->finals);

  print_DFA_transition(Ab);
  print_set(Ab->finals);
  /* Experiments */
}