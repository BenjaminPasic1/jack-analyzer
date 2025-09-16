#include <stdio.h>
#include <string.h>
#include "../include/hashmap.h"

HashMap *create_hashmap(int capacity){
    HashMap *map = malloc(sizeof(HashMap));
    map->entries = calloc(capacity, sizeof(HashEntry*));
    map->capacity = capacity;
    map->size = 0;

    return map;
}

HashEntry *insert_hashmap(HashMap *map, char *key, char *value){
    HashEntry *entry = malloc(sizeof(HashEntry));
    entry->key = strdup(key);
    entry->value = strdup(value);
    entry->next = NULL;

    unsigned long hash = hash_function(key) % map->capacity;

    entry->next = map->entries[hash];
    map->entries[hash] = entry;

    map->size++;

    return entry;
}

void print_hashmap(HashMap *map){
    for(size_t i = 0; i < map->capacity; i++){
	HashEntry *entry = map->entries[i];

	if(entry == NULL)
	    continue;

	while(entry != NULL){
	    printf("index: %zu ; key: %s ; value: %s\n", i, entry->key, entry->value);
	    entry = entry->next;
	}
    }
}

HashEntry *find_hashmap(HashMap *map, char* key){
    unsigned long index = hash_function(key) % map->capacity;

    HashEntry *entry = map->entries[index];

    while(entry != NULL){
	if(strcmp(key, entry->key) == 0){
	    return entry;
	}
	entry = entry->next;
    }
    
    printf("Entry with the given key not found.\n");
    return NULL;
}

unsigned long hash_function(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash =  ((hash << 5) + hash) + c ; 
    }

    return hash;
}

bool has_key(HashMap *map, char *key){
    unsigned long index = hash_function(key) % map->capacity;
    
    HashEntry *curr = map->entries[index];

    if(curr == NULL){
	return false;
    }

    while(curr != NULL){
	
	if(strcmp(key, curr->key) == 0){
	    return true;
	}

	curr = curr->next;
    }

    return false;
}
