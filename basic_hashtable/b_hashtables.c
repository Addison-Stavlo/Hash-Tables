#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Basic hash table key/value pair
 ****/
typedef struct Pair
{
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable
{
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht;
  ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(Pair *));
  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair *));
  pair->key = key;
  pair->value = value;

  unsigned int key_index = hash(key, ht->capacity);

  if (ht->storage[key_index] == NULL)
  {
    //nothing here yet
    ht->storage[key_index] = pair;
  }
  else
  {
    if (strcmp(ht->storage[key_index]->key, pair->key) == 0)
    {
      //same key just overwrite value
      ht->storage[key_index] = pair;
    }
    else
    {
      //collision error
      perror("two keys hash to same index, overwriting the old key: ");
      printf("%s\n", ht->storage[key_index]->key);
      Pair *oldPair = ht->storage[key_index];
      ht->storage[key_index] = pair;
      free(oldPair);
    }
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int key_index = hash(key, ht->capacity);
  if (strcmp(ht->storage[key_index]->key, key) == 0)
  {
    // index exists and keys match
    Pair *oldPair = ht->storage[key_index];
    ht->storage[key_index] = NULL;
    free(oldPair);
  }
  else
  {
    perror("key not found");
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int key_index = hash(key, ht->capacity);
  if (ht->storage[key_index] == NULL)
  {
    return NULL;
  }
  else
  {
    return ht->storage[key_index]->value;
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i <= ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      free(ht->storage[i]);
    }
  }
  free(ht);
}

#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));
  printf("%s \n", hash_table_retrieve(ht, "li"));

  hash_table_insert(ht, "line", "GONE GONE OGNE\n");
  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL)
  {
    printf("...gone tomorrow. (success)\n");
  }
  else
  {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
