#include <set>
#include <iostream>
#include <queue>
#include <tuple>
#include <algorithm>
#include <stdio.h>

#include "finite_machine.hpp"
#include "minimization.hpp"
#include "util.hpp"
using namespace std;

set<int> compute_tau(set<int> P, vector<vector<int> >f){
  //printf("check f: ");
  // for(int i =0; i < f.size(); i++){
  //   printf("state %d : ", i+1);
  //   print_vector(f[i]);
  // }
  // printf("\n");
  set<int> result;
  for(auto it = P.begin(); it != P.end(); it++){
    int i = *it;
    vector<int> image = f[i-1];
    for(int j =0; j < image.size(); j++){
      result.insert(image[j]);
    }
  }
  return result;
}

bool set_intersect_empty(set<int> A, set<int> B){
  vector<int> intersect;
  auto it = set_intersection(A.begin(), A.end(), B.begin(),
                             B.end(), inserter(intersect, intersect.begin()));
  return (intersect.size() == 0);
}

int get_set_index(vector<set<int> > QQ, set<int> target){
  auto found = find(QQ.begin(), QQ.end(), target);
  return found - QQ.begin() + 1;
}

bool q_contains(vector<set<int> >QQ, set<int> target){
  auto found = find(QQ.begin(), QQ.end(), target);
  return found != QQ.end();
}

/* main algorithm parts*/
NFA *compute_rev(DFA* A){
  NFA* revA = new NFA();
  /* alphabet and dimention do not change */
  revA->size = A->size;
  revA->alpha_size = A->alpha_size;
  /* revert initial states and final states*/
  revA->finals = A->inits;
  revA->inits = A->finals;
  /* revert the transition directions */
  vector<vector<vector<int> > > trans(A->alpha_size,vector<vector<int> >());
  for(int i =0; i<A->alpha_size; i++){
    /*initialize everythign to be NULL*/
    trans[i] = vector<vector<int> >(A->size, vector<int>());
    vector<int> f = A->transitions[i];
    for(int j = 0; j<A->size; j++){
        if(f[j]!= -1){
        trans[i][f[j]-1].push_back(j+1);
      }
    }
  }
  revA->transitions = trans;
  //print_NFA_transition(revA);
  //print_set(revA->set)
  return revA;
}

DFA *determinize(NFA *A){
  DFA *detA = new DFA();
  vector<tuple<int, int, int> > table; //used to keep track of transitions
  /* sigma doesn't change */
  detA->alpha_size = A->alpha_size;
  /* inits is just {1}*/
  set<int> inits = {1};
  detA->inits = inits;
  /* we will add states into finals later...*/
  set<int> finals = {};

  /* get transitions */
  queue<set<int>> active;
  vector<set<int>> QQ;
  set<int> I = A->inits;
  active.push(I);
  QQ.push_back(I);
  if(!set_intersect_empty(I, A->finals)){
    /* the case where initial states are also final states */
    finals.insert(1);
  }
  while(!active.empty()){
    set<int> P = active.front();
    active.pop();
    for(int i=0; i<A->alpha_size; i++){
      vector<vector<int>> f = A->transitions[i];
      set<int> R = compute_tau(P, f);
      if(!q_contains(QQ, R)){
        QQ.push_back(R);
        active.push(R);
        /* add it to our finals if necessary*/
        if(!set_intersect_empty(R, A->finals)){
          finals.insert(get_set_index(QQ, R));
        }
      }
      /* keep track of transition P->R via character i*/
      table.push_back(tuple<int, int, int>(get_set_index(QQ, P),
          get_set_index(QQ, R), i+1));
    }
  }

  detA->finals = finals; //convert to DFA finals
  /* size of the DFA is the size of QQ */
  detA->size = QQ.size();

  /* translate transitions */
  vector<vector<int>> trans(detA->alpha_size, vector<int>(detA->size, 0));

  for(int k =0; k < table.size(); k++){
    int p = get<0>(table[k]);
    int q = get<1>(table[k]);
    int i = get<2>(table[k]);
    trans[i-1][p-1] = q;
  }
  detA->transitions = trans;

  return detA;

}

DFA *Brzozowski(DFA * A){
  /* compute NFA rev(A)*/
  NFA * rev_A = compute_rev(A);

  /* Rabin-Scott Determinization */
  DFA * det_RV = determinize(rev_A);

  /* twice in a row*/
  DFA* result = determinize(compute_rev(det_RV));
  return result;
}
