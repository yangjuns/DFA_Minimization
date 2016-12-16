struct pair_s{
  int i;
  int j;
};

typedef struct pair_s pair_t;

struct entry_s {
	pair_t* key;
	int value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;
	struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;

hashtable_t *ht_create(int size);
void ht_set(hashtable_t *hashtable, int key_i, int key_j, int value);
int ht_get(hashtable_t *hashtable, int key_i, int key_j);
void ht_free(hashtable_t *hashtable);