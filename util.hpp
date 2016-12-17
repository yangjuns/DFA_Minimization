#ifndef UTIL_CPP
#define UTIL_CPP

#include <vector>
#include <set>
#include <stdlib.h>
#include <stdio.h>

#include "finite_machine.hpp"
using std::vector;
using std::set;

/* helper functions */
void print_vector(vector<int> arr);

void print_set(set<int> s);

void print_NFA_transition(NFA* A);

void print_DFA_transition(DFA* A);

#endif