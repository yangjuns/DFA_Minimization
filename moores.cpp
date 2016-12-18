#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map>

#include "finite_machine.hpp"
#include "minimization.hpp"
#include "util.hpp"

vector<vector<int>> raw;
unordered_map<int, vector<int>> int2set;
vector<vector<int>> map;

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

/* until functions */
int sum_vector(vector<int> arr){
  int sum = 0;
  for(int i=0;i<arr.size(); i++){
    sum+= arr[i];
  }
  return sum;
}

unordered_map<int, vector<int>> init_int2set(vector<int> &E){
  vector<vector<int>> temp (E.size(), vector<int>(E.size(), 0));
  for(int i =0; i<E.size(); i++){
    int val = E[i];
    temp[val-1][i] = 1;
  }
  raw = temp;
  unordered_map<int, vector<int>> int2set;
  int state = 1;
  for(int i =0; i< temp.size(); i++){
    if(sum_vector(temp[i]) != 0){
      int2set.insert(pair<int, vector<int>>(state, temp[i]));
      state++;
    }
  }
  return int2set;
}

void prep_state_merging(vector<int> &E){
  int2set = init_int2set(E);
  map = vector<vector<int>> (E.size());
  for(int i =0; i<map.size(); i++){
    map[i] = raw[E[i]-1];
  }
}

vector<int> get_equi_class(int state){
  return map[state-1];
}

int get_isom_state(vector<int> equi){
  for(auto it = int2set.begin(); it !=int2set.end(); it++){
    if(it->second == equi){
      return it->first;
    }
  }
  return -1;
}

vector<int> get_equi_class_represented(int n_state){
  auto found = int2set.find(n_state);
  return found->second;
}


/* main algorithm */
vector<int> compute_rho_f(vector<int> &f, vector<int> &rho){
  vector<int> result(f.size());
  for(int i =0;i <f.size();i++){
    result[i] = rho[f[i]-1];
  }
  return result;
}

vector<int> meet(vector<int> &r, vector<int> &s){
  unordered_map<pair<int,int>, int, pairhash> table;
  int p, i ,j;

  vector<int> t(r.size());
  for(p = 0; p < r.size(); p++){
    i = r[p];
    j = s[p];
    auto found = table.find(pair<int, int>(i, j));
    if(found == table.end()){
      pair<pair<int, int>, int> newVal(pair<int, int>(i, j), p+1);
      t[p] = p + 1;
      table.insert(newVal);
    }else{
      t[p] = found->second;
    }
  }
  return t;
}

vector<int> comp_behav_equ(DFA * A){
  /* initialize E0 */
  int F;
  for(int i =0; i< A->finals.size();i++){
    if(A->finals[i] == 1){
      F = i+1;
      break;
    }
  }
  int non_F = F == 1? 2 : 1;
  vector<int> E0(A->size);
  for(int i =0; i< A->size; i++){
    /*check if it is a final state*/
    E0[i] = A->finals[i] == 1? F : non_F;
  }
  /* compute rho_a rho_b */
  vector<int> E_next = E0;
  do{
    E0 = E_next;
    for(int i =0; i< A->transitions.size();i++){
      vector<int> rho_a = compute_rho_f(A->transitions[i], E0);
      E_next = meet(rho_a, E_next);
    }
  }while(E0 != E_next);
  return E_next;
}

DFA* Moores(DFA* A){
  DFA* minA = new DFA();

  /* alphabet size doesnt change */
  minA->alpha_size = A->alpha_size;

  /* computing behavioral equivalence */
  vector<int> E = comp_behav_equ(A);
  // printf("E0 : ");
  // print_vector(E);
  /* state merging */
  prep_state_merging(E);
  // printf("seee------------------\n");
  // for(auto it = int2set.begin(); it !=int2set.end(); it++){
  //   printf("%d: ", it->first);
  //   print_vector(it->second);
  // }

  /* new size*/
  minA->size = int2set.size();
  /* new initials*/
  int ini;
  for(int i =0; i<A->inits.size(); i++){
    if(A->inits[i] == 1){
      ini = i+1;
      break;
    }
  }
  int new_ini = get_isom_state(get_equi_class(ini));
  vector<int> new_ini_set(minA->size, 0);
  new_ini_set[new_ini-1] = 1;
  // printf("new_inits: ");
  // print_vector(new_ini_set);
  minA->inits = new_ini_set;

  /* new finals */
  vector<int> finals(minA->size, 0);
  for(int i =0; i< A->finals.size(); i++) {
    if(A->finals[i] == 1){
      int f_state = get_isom_state(get_equi_class(i+1));
      finals[f_state-1] = 1;
    }
  }
  // printf("new_final_inits: ");
  // print_vector(finals);
  minA->finals = finals;

  /* new trnsitions */
  vector<vector<int>> trans(minA->alpha_size);
  for(int i =0; i<trans.size(); i++){
    trans[i] = vector<int> (minA->size);
    vector<int> origin = A->transitions[i];
    for(int j =0; j <trans[i].size(); j++){
      vector<int> equi_class = get_equi_class_represented(j+1);
      int state;
      for(int k =0; k< equi_class.size(); k++){
        if(equi_class[k]==1){
          state = k+1;
          break;
        }
      }
      int next_state = origin[state-1];
      vector<int> next_equi_class = get_equi_class(next_state);
      int next_isom_state = get_isom_state(next_equi_class);
      trans[i][j] = next_isom_state;
    }
  }
  minA->transitions = trans;
  return minA;
}