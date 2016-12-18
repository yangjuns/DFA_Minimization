#ifndef FINITE_MACHINE_HPP
#define FINITE_MACHINE_HPP

#include <set>
#include <vector>

using namespace std;

/* Representation of a DFA*/
struct DFA{
  int alpha_size;
  int size;
  /* an alpha_size by size matrix */
  vector<vector<int>> transitions;
  vector<int> inits; /*bit vector of length size. ints[i] = 1 means state i+1 is inital state*/
  vector<int> finals;
};



/* Representation of a NFA*/

struct NFA{
  int size;
  int alpha_size;
  vector<vector<vector<int>>> transitions;
  vector<int> inits;
  vector<int> finals;
};

#endif