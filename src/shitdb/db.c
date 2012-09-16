#include <stdlib.h>
#include <shitdb/dbg.h>
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
