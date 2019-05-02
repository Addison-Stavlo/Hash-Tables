#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
  int size;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = malloc(strlen(key) * (sizeof(char)));
  pair->value = malloc(strlen(value) * sizeof(char));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht;
  ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  LinkedPair *newPair = create_pair(key, value);

  unsigned int key_index = hash(key, ht->capacity);

  if (ht->storage[key_index] == NULL)
  {
    ht->storage[key_index] = newPair;
  }
  else
  {
    if (strcmp(ht->storage[key_index]->key, newPair->key) == 0)
    {
      //same key just overwrite value
      ht->storage[key_index] = newPair;
    }
    else
    {
      //collision error insert new Pair at end of linked list chain
      LinkedPair *prev = NULL;
      LinkedPair *current = ht->storage[key_index];

      // traverse to end of LL chain
      while (current)
      {
        prev = current;
        current = current->next;
      }
      prev->next = newPair;
    }
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int key_index = hash(key, ht->capacity);
  if (strcmp(ht->storage[key_index]->key, key) == 0)
  {
    // first element matches key
    LinkedPair *oldPair = ht->storage[key_index];
    ht->storage[key_index] = oldPair->next;
    destroy_pair(oldPair);
  }
  else if (ht->storage[key_index]->key)
  {
    LinkedPair *prev = ht->storage[key_index];
    LinkedPair *current = ht->storage[key_index]->next;

    //search LL for matching key
    while (current)
    {
      if (strcmp(current->key, key) == 0)
      {
        prev->next = current->next;
        destroy_pair(current);
        return;
      }
      prev = current;
      current = current->next;
    }
    perror("key not found");
  }
  else
  {
    perror("key not found");
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int key_index = hash(key, ht->capacity);
  if (ht->storage[key_index] == NULL)
  {
    return NULL;
  }
  else
  {
    LinkedPair *current = ht->storage[key_index];

    //search LL for matching key
    while (current)
    {
      if (strcmp(current->key, key) == 0)
      {
        return current->value;
      }
      current = current->next;
    }
    perror("key not found");
    return NULL;
  }
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i <= ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *current = ht->storage[i];
      LinkedPair *toDelete = NULL;
      //search LL for matching key
      while (current)
      {
        toDelete = current;
        current = current->next;
        destroy_pair(toDelete);
      }
    }
  }
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity * 2);

  // go thorugh old hash table indexs and LLs, insert every element to new hash table

  for (int i = 0; i <= ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *current = ht->storage[i];
      LinkedPair *toInsert = NULL;
      while (current)
      {
        toInsert = current;
        current = current->next;
        hash_table_insert(new_ht, toInsert->key, toInsert->value);
        destroy_pair(toInsert);
      }
    }
    ht->storage[i] = NULL;
  }
  destroy_hash_table(ht);

  return new_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
