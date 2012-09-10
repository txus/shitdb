#ifndef _object_
#define _object_

#include <shitdb/bstrlib.h>

typedef enum {
  tInteger,
  tString
} Type;

typedef struct Object {
  Type type;
  union {
    int as_integer;
    bstring as_string;
  } value;
} Object;

Object *Object_create_integer(int value);
Object *Object_create_string(bstring value);
Object *Object_destroy(Object *object);
bstring Object_to_string(Object *object);
Object *String_to_object(bstring string);

#endif
