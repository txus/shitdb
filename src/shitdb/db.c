#include <stdlib.h>
#include <shitdb/dbg.h>
#include <shitdb/hashmap.h>
#include <shitdb/db.h>

static void destroy_object(void *obj)
{
  Object_destroy((Object*)obj);
}

static int destroy_object_node(HashmapNode *node)
{
  destroy_object((Object*)node);
  return 0;
}

DB*
DB_create()
{
  DB *db = calloc(1, sizeof(DB));
  check_mem(db);

  db->map = Hashmap_create(NULL, NULL, destroy_object);
  return db;

error:
  return NULL;
}

void
DB_clear(DB *db)
{
  Hashmap_traverse(db->map, destroy_object_node);
}

void
DB_delete(DB *db, bstring key)
{
  Hashmap_delete(db->map, key);
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

  if (array == NULL) {
    array = Object_create_array(DArray_create(sizeof(Object*), 1));
    Hashmap_set(db->map, key, array);
  }

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
    hash = Object_create_hash(Hashmap_create(NULL, NULL, destroy_object));
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

