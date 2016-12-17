#include "set.h"
/* Representation of a DFA*/
struct DFA_s{
  int alpha_size;
  int size;
  int** transitions;
  set_t* inits;
  set_t* finals;
};

typedef struct DFA_s DFA_t;


/* Representation of a NFA*/
struct state_s{
  int state;
  struct state_s *next;
};

typedef struct state_s state_t;

struct NFA_s{
  int size;
  int alpha_size;
  state_t ***transitions;
  set_t* inits;
  set_t* finals;
};

typedef struct NFA_s NFA_t;