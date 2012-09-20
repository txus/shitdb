#include <stdlib.h>
#include <string.h>
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

Object*
Object_create_array(DArray *array)
{
  Object *obj = Object_allocate();
  obj->type = tArray;
  obj->value.as_array = array;
  return obj;
}

Object*
Object_create_hash(Hashmap *hash)
{
  Object *obj = Object_allocate();
  obj->type = tHash;
  obj->value.as_hash = hash;
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
    case tArray: {
      DArray *ary = object->value.as_array;
      str = calloc(1024, sizeof(char));

      strcat(str, "[");

      int i = 0;
      int count = DArray_count(ary);

      for(i=0; i < count; i++) {
        bstring elm = Object_to_string(DArray_get(ary, i));
        strcat(str, bdata(elm));
        if(i != count - 1) strcat(str, ", ");
      }

      strcat(str, "]");
      break;
    }
    default: {
      str = calloc(4, sizeof(char));
      sprintf(str, "nil");
      break;
    }
  }

  return S(str);
}

Object*
String_to_object(bstring string)
{
  Object *obj = NULL;

  if (bchar(string, 0) == '"') {
    int len = blength(string) - 2;
    obj = Object_create_string(bmidstr(string, 1, len));
  } else if (bchar(string, 0) == '[') {
    int strlen = blength(string) - 2;
    bstring inner_str = bmidstr(string, 1, strlen);
    struct bstrList *elements = bsplit(inner_str, ',');

    int len = elements->qty;
    int i = 0;

    DArray *array = DArray_create(sizeof(Object*), len);
    bstring *ptr = elements->entry;
    for(i = 0; i < len; i++) {
      btrimws(*ptr);
      DArray_push(array, String_to_object(*ptr));
      ptr++;
    }
    obj = Object_create_array(array);
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
