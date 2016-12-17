
#include "minimization.hpp"
#include "util.cpp"

int main(){
  DFA* A = new DFA();
  vector<vector<int> > trans = {{2,2,5,6,5,6},{3,4,6,5,5,6}};
  A->alpha_size = 2;
  A->size = 6;
  A->transitions = trans;
  set<int> inits = {1};
  A->inits = inits;
  set<int> finals = {4};
  A->finals = finals;
  DFA* Am = Moores(A);
  DFA* Ab = Brzozowski(A);
  print_DFA_transition(Am);
  print_set(Am->finals);
  print_DFA_transition(Ab);
  print_set(Ab->finals);

}