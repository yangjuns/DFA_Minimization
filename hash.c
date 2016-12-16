#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "hash.h"


/* Create a new hashtable. */
hashtable_t *ht_create(int size) {

	hashtable_t *hashtable = NULL;
	int i;

	if(size < 1) return NULL;

	/* Allocate the table itself. */
	if((hashtable = malloc(sizeof(hashtable_t))) == NULL) {
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if((hashtable->table = malloc(sizeof(entry_t *) * size)) == NULL) {
		return NULL;
	}
	for(i = 0; i < size; i++) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;
	return hashtable;
}

/* Hash a string for a particular hash table. */
int ht_hash(hashtable_t *hashtable, pair_t *key) {
  int hashval = key->i + key->j;
	return hashval % hashtable->size;
}

int key_compare(pair_t* key1, pair_t* key2){
  return key1->i == key2->i && key1->j == key2->j;
}

/* Create a key-value pair. */
entry_t *ht_newpair(pair_t *key, int value) {
	entry_t *newpair;

	if((newpair = malloc(sizeof(entry_t))) == NULL) {
		return NULL;
	}
	newpair->key = key;
  newpair->value = value;
	newpair->next = NULL;

	return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set(hashtable_t *hashtable, int key_i, int key_j, int value) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;
  pair_t* key = malloc(sizeof(pair_t));
  key->i = key_i;
  key->j = key_j;
	bin = ht_hash(hashtable, key);

	next = hashtable->table[bin];

	while(next != NULL && key_compare(key, next->key) != 1) {
		last = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that int. */
	if(next != NULL) {
    free(key);
		next->value = value;
	/* Nope, could't find it.  Time to grow a pair. */
	} else {
		newpair = ht_newpair(key, value);

		/* We're at the start of the linked list in this bin. */
		if(next == hashtable->table[bin]) {
			newpair->next = next;
			hashtable->table[bin] = newpair;

		/* We're at the end of the linked list in this bin. */
		} else if (next == NULL) {
			last->next = newpair;

		/* We're in the middle of the list. */
		} else  {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

/* Retrieve a key-value pair from a hash table. */
int ht_get(hashtable_t *hashtable, int key_i, int key_j) {
	int bin = 0;
	entry_t *pair;
  pair_t* key = malloc(sizeof(pair_t));
  key->i = key_i;
  key->j = key_j;
  bin = ht_hash(hashtable, key);
	/* Step through the bin, looking for our value. */
	pair = hashtable->table[bin];
	while(pair != NULL && key_compare(key, pair->key) != 1) {
		pair = pair->next;
	}
  free(key);
	/* Did we actually find anything? */
	if(pair == NULL) {
		return -1;
	} else {
		return pair->value;
	}
}

/* Free the hash table */
void ht_free(hashtable_t *hashtable){
  if(hashtable != NULL){
    for(int i =0; i< hashtable->size; i++){
      entry_t* t = hashtable->table[i];
      entry_t* prev = NULL;
      for(;t != NULL; t=t->next){
        free(t->key);
        if(prev) free(prev);
        prev = t;
      }
      if(prev) free(prev);
    }
    free(hashtable->table);
    free(hashtable);
  }
}