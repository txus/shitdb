#ifndef _lcthw_Hashmap_h
#define _lcthw_Hashmap_h

#include <stdint.h>
#include <shitdb/darray.h>

#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*Hashmap_compare)(void *a, void *b);
typedef uint32_t (*Hashmap_hash)(void *key);
typedef void (*Hashmap_destroy_cb)(void *node);

typedef struct Hashmap {
  DArray *buckets;
  Hashmap_compare compare;
  Hashmap_hash hash;
  Hashmap_destroy_cb destroy;
} Hashmap;

typedef struct HashmapNode {
  void *key;
  void *data;
  uint32_t hash;
} HashmapNode;

typedef int (*Hashmap_traverse_cb)(HashmapNode *node);

Hashmap *Hashmap_create(Hashmap_compare, Hashmap_hash, Hashmap_destroy_cb);
void Hashmap_destroy(Hashmap *map);

int Hashmap_set(Hashmap *map, void *key, void *data);
void *Hashmap_get(Hashmap *map, void *key);

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb);

void *Hashmap_delete(Hashmap *map, void *key);

int reverse_compare(void *a, void *b);

#endif
