#include <set>
#include <vector>

using namespace std;

/* Representation of a DFA*/
struct DFA{
  int alpha_size;
  int size;
  /* an alpha_size by size matrix */
  vector<vector<int>> transitions;
  set<int> inits;
  set<int> finals;
};



/* Representation of a NFA*/

struct NFA{
  int size;
  int alpha_size;
  vector<vector<vector<int>>> transitions;
  set<int> inits;
  set<int> finals;
};
