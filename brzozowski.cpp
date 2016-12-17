#include <set>

void print_arr(int* arr, int n){
  for(int i = 0; i < n;i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void print_NFA_transition(NFA_t * A){
  state_t *** T = A->transitions;
  for(int i =0; i < A->alpha_size; i++){
    printf("----------f%d-------\n", i);
    state_t** t = T[i];
    for(int j =0; j< A->size; j++){
      state_t* p = t[j];
      printf("state %d : ", j+1);
      while(p){
        printf(" %d, ", p->state);
        p=p->next;
      }
      printf("\n");
    }
  }
}

NFA_t *compute_rev(DFA_t *A){
  NFA_t *revA = malloc(sizeof(NFA_t));
  /* alphabet and dimention do not change */
  revA->size = A->size;
  revA->alpha_size = A->alpha_size;
  /* revert initial states and final states*/
  revA->finals = A->inits;
  revA->inits = A->finals;
  /* revert the transition directions */
  state_t ***trans = malloc(sizeof(state_t **) * revA->alpha_size);
  for(int i =0; i<A->alpha_size; i++){
    trans[i] = malloc(sizeof(state_t *) * revA->size);
    /*initialize everythign to be NULL*/
    memset(trans[i], 0, revA->size * sizeof(state_t *));
    int* f = A->transitions[i];
    print_arr(f, 4);
    for(int j = 0; j<A->size; j++){
      //printf("%d ", f[j]);
      if(f[j]!= -1){
        state_t * state = malloc(sizeof(state_t));
        state->state = j+1;
        /*add it to the front*/
        state->next = trans[i][f[j]-1];
        trans[i][f[j]-1] = state;
      }
    }
  }
  revA->transitions = trans;
  print_NFA_transition(revA);
}

set_t *compute_tau(set_t* P, state_t **f){
  /* edge case */
  if(f == NULL) return NULL;

  int temp [1024];
  int count = 0;
  for(int i =0; i < P->size; i++){
    state_t* neibors = f[P->set[i]];
    while(neibors){
      temp[count] = neibors->state;
      neibors = neibors->next;
      count++;
    }
  }
  int* set = malloc(sizeof(int)*count);
  memcpy(set, temp, sizeof(int)*count);
  set_t* result = malloc(sizeof(set_t));
  result->set = set;
  result->size = count;
  return result;
}

DFA_t *determinize(NFA_t *A){
  DFA_t *detA = malloc(sizeof(DFA_t));
  set_t *inits, *finals;
  hashtable_t trans = ht_create(100); //used to keep track of transitions
  /* sigma doesn't change */
  detA->alpha_size = A->alpha_size;
  /* inits is just {1}*/
  int* set = malloc(sizeof(int));
  set[0] = 1;
  inits = set_init(set);
  detA->inits = inits;
  /* we will add states into finals later...*/
  finals = set_init(NULL);

  /* get transitions */
  queue_t *active = init_queue();
  queue_t *QQ = init_queue();
  set_t *I = A->inits;
  enque(active, I);
  if(!set_intersect_empty(I, A->finals)){
    /* the case where initial states are also final states */
    set_add(finals, q_get_seq(QQ, I));
  }
  enque(QQ, I);
  while(!is_empty(active)){
    set_t *P = deque(active);
    for(int i=0; i<A->alpha_size; i++){
      state_t** f = A->transitions[i];
      set_t* R = compute_tau(P, f);
      if(!q_contains(QQ, R)){
        enque(QQ,R);
        /* add it to our finals if necessary*/
        if(!set_intersect_empty(R, A->finals)){
          set_add(finals, q_get_seq(QQ, R));
        }
      }
      /* keep track of transition P->R via character i*/
      ht_set(trans, q_get_seq(QQ,P), q_get_seq(QQ, R), i);
    }
  }

  detA->finals = finals; //convert to DFA finals

  /* size of the DFA is the size of QQ */
  detA->size = q_size(QQ);
  /* translate transitions */
  int** trans = malloc(sizeof(int *) * detA->alpha_size);
  for(int i = 0; i < defA->alpha_size; i++){
    trans[i] = malloc(sizeof(int) * detA->size);
  }

  entry_t* list = ht_convert2list(trans);
  while(list){
    trans[list->value][list->key->i] = list->key->j;
  }
  detA->transitions = trans;
  return detA;
}

DFA_t *brzozowski(DFA_t * A){
  /* compute NFA rev(A)*/
  NFA_t * rev_A = compute_rev(A);

  /* Rabin-Scott Determinization */
  DFA_t * det_RV = determinize(rev_A);
}







int main(){
  DFA_t * A = malloc(sizeof(DFA_t *));
  int temp[2][4] ={{2,3,4,4},{2,3,-1,4}};
  int** transitions = malloc(sizeof(int *)*2);
  for(int i = 0; i< 2; i++){
    transitions[i] = malloc(sizeof(int) *4);
    memcpy(transitions[i], temp[i], sizeof(int)*4);
  }
  printf("%d\n", transitions[0][0]);
  A->alpha_size = 2;
  A->size = 4;
  A->transitions = transitions;
  NFA_t* revA = compute_rev(A);
}