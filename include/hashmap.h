#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct HashEntry{
    char *key;
    char *value;
    struct HashEntry *next; 
} HashEntry;

typedef struct {
    HashEntry **entries;
    int capacity;
    int size;
} HashMap;

HashMap *create_hashmap(int capacity);
HashEntry *insert_hashmap(HashMap *map, char *key, char *value);
HashEntry *find_hashmap(HashMap *map, char *key);
unsigned long hash_function(char *key);
void print_hashmap(HashMap *map);

bool has_key(HashMap *map, char *key);

#endif
