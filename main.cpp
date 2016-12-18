#include <iostream>
#include <chrono>
#include <set>

using namespace std;
using namespace std::chrono;

#include "minimization.hpp"
#include "util.cpp"

/* Used to generate random DFA of certain complexity*/
DFA* generate_random_DFA(int size, int alpha_size){
  DFA* A = new DFA();
  A->size = size;
  A->alpha_size = alpha_size;
  vector<int> inits(size, 0);
  inits[0] = 1;
  A->inits = inits;
  /*generate random final sets*/
  vector<int> finals(size);
  for(int i =0; i<size; i++){
    finals[0] = rand() % 2;
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

/* Print average runtime of Moore's algorithm over @runs runs of random DFAs
 * of certain size
 * @param runs        how many random DFAs to minimize
 * @param size        the number of states of these DFAs
 * @param alpha_size  the size of the alphabet
 **/
void run_moore(int runs, int size, int alpha_size){
  int n = runs;
  int i = 0;
  int m_sum = 0;
  while(i < n){
    DFA* A = generate_random_DFA(size, alpha_size);
    high_resolution_clock::time_point t1, t2;
    t1 = high_resolution_clock::now();
    Moores(A);
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    m_sum += duration;
    i++;
  }
  printf("Moores AVG Run Time: %d\n", m_sum/runs);
}

/* Print average runtime of Brzozowski's algorithm over @runs runs of random DFAs
 * of certain size
 * @param runs        how many random DFAs to minimize
 * @param size        the number of states of these DFAs
 * @param alpha_size  the size of the alphabet
 **/
void run_brzozowski(int runs, int size, int alpha_size){
  int n = runs;
  int i = 0;
  int b_sum = 0;
  while(i < n){
    DFA* A = generate_random_DFA(size, alpha_size);
    high_resolution_clock::time_point t1, t2;
    t1 = high_resolution_clock::now();
    Brzozowski(A);
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    b_sum += duration;
    i++;
  }
  printf("Brzozowski AVG Run Time: %d\n", b_sum/runs);
}


/* Print average runtime of both algorithms over @runs runs of random DFAs
 * of certain size
 * @param runs        how many random DFAs to minimize
 * @param size        the number of states of these DFAs
 * @param alpha_size  the size of the alphabet
 **/
void compare_runtime(int runs, int size, int alpha_size){
  run_moore(runs, size, alpha_size);
  run_brzozowski(runs, size, alpha_size);
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
  vector<int> inits = {1,0,0,0,0,0};
  A->inits = inits;
  vector<int> finals = {0,0,1,1,0,0};
  A->finals = finals;
  print_DFA_transition(A);
  printf("FS: ");
  print_vector(A->finals);
  printf("=========================\n");
  t1 = high_resolution_clock::now();
  DFA* Am = Moores(A);
  t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count();
  printf("Moores Run Time: %ld\n", duration);
  printf("Output: \n");
  print_DFA_transition(Am);
  printf("Final states: ");
  print_vector(Am->finals);
  printf("=========================\n");

  t1 = high_resolution_clock::now();
  DFA* Ab = Brzozowski(A);
  t2 = high_resolution_clock::now();
  duration = duration_cast<microseconds>( t2 - t1 ).count();
  printf("Brzozowski Run Time: %ld\n", duration);
  printf("Output: \n");
  print_DFA_transition(Ab);
  printf("Final states: ");
  print_vector(Ab->finals);
  printf("==========================\n");

  /* Experiments */
  printf("Experiments with lots of input ...\n");
  /*Moore's Algorithm */
  printf("==========================\n");
  printf("Moore's Algorithm runtime Experiments: \n");
  run_moore(10, 50, 2);
  run_moore(10, 500, 2);
  run_moore(10, 50, 20);
  printf("==========================\n");
  printf("Brzozowski's Algorithm runtime Experiments: \n");
  run_brzozowski(2, 20, 2);
  printf("It grows from exponentially so I skipped it \n");
  printf("==========================\n");
  printf("Compareing both algorithms:\n");
  compare_runtime(10, 10, 2);
  compare_runtime(10, 20, 2);
  compare_runtime(10, 10, 20);
}