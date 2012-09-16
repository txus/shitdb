#ifndef _db_
#define _db_

#include <shitdb/bstrlib.h>
#include <shitdb/hashmap.h>
#include <shitdb/object.h>

typedef struct DB {
  Hashmap *map;
} DB;

DB* DB_create();
#define DB_destroy(db) Hashmap_destroy(db->map); free(db);
void DB_set(DB *db, bstring key, Object *value);
Object* DB_get(DB *db, bstring key);

// Array
void DB_apush(DB *db, bstring key, Object *value);
Object* DB_apop(DB *db, bstring key);

#endif
