#include <stdlib.h>
#include <shitdb/dbg.h>
#include <shitdb/hashmap.h>
#include <shitdb/db.h>

DB*
DB_create()
{
  DB *db = calloc(1, sizeof(DB));
  check_mem(db);

  db->map = Hashmap_create(NULL, NULL);
  return db;

error:
  return NULL;
}

void
DB_set(DB *db, bstring key, Object *value)
{
  Hashmap_set(db->map, key, value);
}

Object*
DB_get(DB *db, bstring key)
{
  return (Object*)Hashmap_get(db->map, key);
}

/*
 * Array operations
 */

void
DB_apush(DB *db, bstring key, Object *value)
{
  Object *array = (Object*)Hashmap_get(db->map, key);
  DArray_push(array->value.as_array, value);
}

Object*
DB_apop(DB *db, bstring key)
{
  Object *array = (Object*)Hashmap_get(db->map, key);
  return (Object*)DArray_pop(array->value.as_array);
}

Object*
DB_aat(DB *db, bstring key, int index)
{
  Object *array = (Object*)Hashmap_get(db->map, key);
  return (Object*)DArray_at(array->value.as_array, index);
}

Object*
DB_acount(DB *db, bstring key)
{
  Object *array = (Object*)Hashmap_get(db->map, key);
  int count = DArray_count(array->value.as_array);
  return Object_create_integer(count);
}

void
DB_hset(DB *db, bstring key, bstring hkey, Object *value)
{
  Object *hash = (Object*)Hashmap_get(db->map, key);

  if (hash == NULL) {
    hash = Object_create_hash(Hashmap_create(NULL, NULL));
    Hashmap_set(db->map, key, hash);
  }

  Hashmap_set(hash->value.as_hash, hkey, value);
}

Object*
DB_hget(DB *db, bstring key, bstring hkey)
{
  Object *hash = (Object*)Hashmap_get(db->map, key);
  if (hash == NULL) {
    return NULL;
  } else {
    return Hashmap_get(hash->value.as_hash, hkey);
  }
}

