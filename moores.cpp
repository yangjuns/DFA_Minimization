#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include "finite_machine.hpp"

vector<set<int>> raw;
unordered_map<int, set<int>> int2set;
vector<set<int>> map;

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};



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
unordered_map<int, set<int>> init_int2set(vector<int> E){
  vector<set<int>> temp (E.size(), set<int>());
  for(int i =0; i<E.size(); i++){
    int val = E[i];
    temp[val-1].insert(i+1);
  }
  raw = temp;
  unordered_map<int, set<int>> int2set;
  int state = 1;
  for(int i =0; i< temp.size(); i++){
    if(temp[i].size() != 0){
      int2set.insert(pair<int, set<int>>(state, temp[i]));
      state++;
    }
  }
  return int2set;
}



/* until functions */
void prep_state_merging(vector<int> E){
  int2set = init_int2set(E);
  map = vector<set<int>> (E.size());
  for(int i =0; i<map.size(); i++){
    map[i] = raw[E[i]-1];
  }
}

set<int> get_equi_class(int state){
  return map[state-1];
}

int get_isom_state(set<int> equi){
  for(auto it = int2set.begin(); it !=int2set.end(); it++){
    if(it->second == equi){
      return it->first;
    }
  }
  return -1;
}

set<int> get_equi_class_represented(int n_state){
  auto found = int2set.find(n_state);
  return found->second;
}


/* main algorithm */
vector<int> compute_rho_f(vector<int> f, vector<int> rho){
  vector<int> result(f.size());
  for(int i =0;i <f.size();i++){
    result[i] = rho[f[i]-1];
  }
  return result;
}

vector<int> meet(vector<int> r, vector<int> s){
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
  printf("----------------\n");
  /* initialize E0 */
  printf("initializing EO ...");
  int F = *(A->finals.begin());
  int non_F = F == 1? 2 : 1;
  vector<int> E0(A->size);
  for(int i =0; i< A->size; i++){
    /*check if it is a final state*/
    auto found = A->finals.find(i+1);
    E0[i] = (found != A->finals.end()) ? F: non_F;
  }
  printf("Done\n");
  printf("E0 : ");
  print_vector(E0);
  /* compute rho_a rho_b */
  vector<int> E_next = E0;
  do{
    E0 = E_next;

    vector<int> rho_a = compute_rho_f(A->transitions[0], E0);
    printf("rho_a: ");
    print_vector(rho_a);
    vector<int> rho_b = compute_rho_f(A->transitions[1], E0);
    printf("rho_b: ");
    print_vector(rho_b);
    // int* R_a = meet(rho_a, E0, size);
    // int* R_b = meet(rho_b, E0, size);
    vector<int> temp = meet(rho_a, rho_b);
    E_next = meet(temp, E0);
    printf("E_nxt: ");
    print_vector(E_next);
  }while(E0 != E_next);
  printf("------------------\n");
  return E_next;
}


DFA* Moores(DFA* A){
  DFA* minA = new DFA();

  /* alphabet size doesnt change */
  minA->alpha_size = A->alpha_size;

  /* computing behavioral equivalence */
  printf("Computing behavioral equivalence....\n");
  vector<int> E = comp_behav_equ(A);
  printf("Finished computing.\n");
  /* state merging */
  prep_state_merging(E);
  /* new size*/
  minA->size = int2set.size();

  /* new initials*/
  int ini = *(A->inits.begin());
  int new_ini = get_isom_state(get_equi_class(ini));
  minA->inits = {new_ini};

  /* new finals */
  set<int> finals ={};
  for(auto it = A->finals.begin(); it != A->finals.end();it++) {
    int i = *it;
    int f_state = get_isom_state(get_equi_class(i));
    finals.insert(f_state);
  }
  minA->finals = finals;

  /* new trnsitions */
  vector<vector<int>> trans(minA->alpha_size);
  for(int i =0; i<trans.size(); i++){
    trans[i] = vector<int> (minA->size);
    vector<int> origin = A->transitions[i];
    for(int j =0; j <trans[i].size(); j++){
      set<int> equi_class = get_equi_class_represented(j+1);
      int state = *(equi_class.begin());
      int next_state = origin[state-1];
      set<int> next_equi_class = get_equi_class(next_state);
      int next_isom_state = get_isom_state(next_equi_class);
      trans[i][j] = next_isom_state;
    }
  }
  minA->transitions = trans;
  return minA;
}

int main(){
  // int r[6] = {1,1,1,1,1,1};
  // int s[6] = {3,3,1,1,1,1};
  // int e[6] = {3,4,1,2,4,5};
  // int* t1 = meet(e, e, 6);
  // print_arr(t1, 6);
  // int* t2  = meet(s, e, 6);
  // print_arr(t2, 6);
  // int* t  = meet(t1, t2, 6);
  // print_arr(t, 6);
  //
  // int matrix[2][8] = {{2,4,5,2,6,8,4,6}, {3,5,4,3,7,4,8,7}};
  // int finals[] = {1,4};
  // int* answer = Moores(8, matrix, finals, 2);
  // printf("answer:\n");
  // print_arr(answer, 8);
  // /* Need to free things*/

  DFA* A = new DFA();
  vector<vector<int> > trans = {{2,3,4,4},{2,3,-1,4}};
  A->alpha_size = 2;
  A->size = 4;
  A->transitions = trans;
  set<int> inits = {1};
  A->inits = inits;
  set<int> finals = {4};
  A->finals = finals;
  DFA* minA = Moores(A);
  print_DFA_transition(minA);
  print_set(minA->finals);
}