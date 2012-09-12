#include <stdlib.h>
#include <shitdb/dbg.h>
#include <shitdb/db.h>
#include <shitdb/object.h>

Object*
Object_allocate()
{
  Object *obj = calloc(1, sizeof(Object));
  check_mem(obj);
  obj->type = tNil;
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

bstring
Object_to_string(Object *object)
{
  char *str = NULL;

  switch(object->type) {
    case tInteger: {
      str = calloc(100, sizeof(char));
      sprintf(str, "%i", object->value.as_integer);
      break;
    }
    case tString: {
      int size = blength(object->value.as_string);
      str = calloc(size, sizeof(char));

      sprintf(str, "\"%s\"", bdata(object->value.as_string));
      break;
    }
    default: {
      str = calloc(4, sizeof(char));
      sprintf(str, "nil");
      break;
    }
  }

  return bfromcstr(str);
}

Object*
String_to_object(bstring string)
{
  Object *obj = NULL;

  if (bchar(string, 0) == '"') {
    int len = blength(string) - 2;
    obj = Object_create_string(bmidstr(string, 1, len));
  } else {
    int value = atoi(bdata(string));
    if (value != 0) {
      obj = Object_create_integer(atoi(bdata(string)));
    } else {
      return NULL;
    }
  }
  return obj;
}
