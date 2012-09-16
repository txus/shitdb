#ifndef _object_
#define _object_

#include <shitdb/bstrlib.h>
#include <shitdb/darray.h>

typedef enum {
  tNil,
  tInteger,
  tString,
  tArray,
} Type;

typedef struct Object {
  Type type;
  union {
    int as_integer;
    bstring as_string;
    DArray *as_array;
  } value;
} Object;

Object *Object_allocate();
Object *Object_create_integer(int value);
Object *Object_create_string(bstring value);
Object *Object_create_array(DArray *array);
Object *Object_destroy(Object *object);
bstring Object_to_string(Object *object);
Object *String_to_object(bstring string);

#endif
