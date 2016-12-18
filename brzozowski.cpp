#include <set>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>

#include "finite_machine.hpp"
#include "minimization.hpp"
#include "util.hpp"
using namespace std;


/******************  helper functions *****************/
struct sethash {
public:
  size_t operator()(const vector<int> &x) const
  {
    if(x.size()>0){
      return x.size() * (x[0] + x.back());
    }else{
      return 0;
    }
  }
};

unordered_map<vector<int>, int, sethash> set2int;

vector<int> compute_tau(vector<int> &P, vector<vector<int> > &f){
  vector<int> result(P.size(), 0);
  for(int i=0 ;i<P.size(); i++){
    if(P[i] == 1){
      for(int j=0; j<result.size(); j++){
        result[j] = result[j] | f[i][j];
      }
    }
  }
  return result;
}

bool set_intersect_empty(vector<int> &A, vector<int> &B){
  vector<int> intersect (A.size(), 0);
  for(int i = 0;i <A.size();i++){
    if(A[i] & B[i] == 1) return true;
  }
  return false;
}

bool q_contains(vector<vector<int>> &QQ, vector<int> &target){
  auto found = find(QQ.begin(), QQ.end(), target);
  return found != QQ.end();
}

int get_set_state(vector<int> &s){
  auto found = set2int.find(s);
  return found->second;
}
/******* END of helper functions *****/


/****************** main algorithms *****************/
NFA *compute_rev(DFA* A){
  NFA* revA = new NFA();
  /* alphabet and dimention do not change */
  revA->size = A->size;
  revA->alpha_size = A->alpha_size;
  /* revert initial states and final states*/
  revA->finals = A->inits;
  revA->inits = A->finals;
  /* revert the transition directions */
  vector<vector<vector<int>>> trans(A->alpha_size,vector<vector<int>>());
  for(int i =0; i<A->alpha_size; i++){
    /*initialize everythign to be NULL*/
    trans[i] = vector<vector<int>> (A->size, vector<int>(A->size, 0));
    vector<int> f = A->transitions[i];
    for(int j = 0; j<A->size; j++){
      if(f[j]!= -1){
        trans[i][f[j]-1][j] = 1;
      }
    }
  }
  revA->transitions = trans;
  return revA;
}

DFA *determinize(NFA *A){
  DFA *detA = new DFA();
  vector<tuple<int, int, int>> table; //used to keep track of transitions
  /* sigma doesn't change */
  detA->alpha_size = A->alpha_size;
  /* get transitions */
  int count = 1;
  queue<vector<int>> active;
  vector<vector<int>> QQ;
  vector<int> F; //used later to construct finals
  vector<int> I = A->inits;
  active.push(I);
  QQ.push_back(I);
  set2int.insert(pair<vector<int>,int>(I, count));
  count++;
  if(!set_intersect_empty(I, A->finals)){
    /* the case where initial states are also final states */
    F.push_back(1);
  }
  while(!active.empty()){
    vector<int> P = active.front();
    active.pop();
    for(int i=0; i<A->alpha_size; i++){
      vector<vector<int>> f = A->transitions[i];
      vector<int> R = compute_tau(P, f);
      if(!q_contains(QQ, R)){
        QQ.push_back(R);
        set2int.insert(pair<vector<int>,int>(R, count));
        count++;
        active.push(R);
        /* add it to our finals if necessary*/
        if(!set_intersect_empty(R, A->finals)){
          F.push_back(get_set_state(R));
        }
      }
      /* keep track of transition P->R via character i*/
      table.push_back(tuple<int, int, int>(get_set_state(P),
          get_set_state(R), i+1));
    }
  }

  /* size of the DFA is the size of QQ */
  detA->size = QQ.size();
  /* initils is just {1} */
  vector<int> inits(detA->size, 0);
  inits[0]=1;
  detA->inits = inits;
  /* construct finals bit vector*/
  vector<int> finals(detA->size, 0);
  for(int i =0; i<F.size(); i++){
    finals[F[i]-1] = 1;
  }
  detA->finals = finals;
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
  set2int.clear();
  /* compute NFA rev(A)*/
  NFA * rev_A = compute_rev(A);
  /* Rabin-Scott Determinization */
  DFA * det_RV = determinize(rev_A);
  set2int.clear();
  /* twice in a row*/
  NFA * rev_A2 = compute_rev(det_RV);
  DFA* result = determinize(rev_A2);
  return result;
}
