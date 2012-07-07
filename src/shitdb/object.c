#include <stdlib.h>
#include <shitdb/dbg.h>
#include <shitdb/db.h>

static inline Object*
Object_allocate()
{
  Object *obj = calloc(1, sizeof(Object));
  check_mem(obj);
  return obj;
error:
  return NULL;
}

Object*
Object_create_integer(int value)
{
  Object *obj = Object_allocate();
  obj->type = tInteger;
  obj->value.as_integer = value;
  return obj;
}

Object*
Object_create_string(bstring value)
{
  Object *obj = Object_allocate();
  obj->type = tString;
  obj->value.as_string = value;
  return obj;
}
