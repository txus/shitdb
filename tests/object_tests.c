#include "minunit.h"
#include <shitdb/object.h>

char *test_to_string()
{
  Object *integer = Object_create_integer(123);
  bstring int_result = Object_to_string(integer);

  Object *string = Object_create_string(bfromcstr("hello"));
  bstring str_result = Object_to_string(string);

  mu_assert(bstrcmp(int_result, bfromcstr("123")) == 0, "Integer to string mismatch");
  mu_assert(bstrcmp(str_result, bfromcstr("\"hello\"")) == 0, "String obj to string mismatch");

  return NULL;
}

char *test_to_object()
{
  bstring integer = bfromcstr("1");
  bstring string = bfromcstr("\"hello\"");

  Object *obj_int = String_to_object(integer);
  Object *obj_str = String_to_object(string);

  mu_assert(obj_int->type == tInteger, "Integer to object is not of type Integer");
  mu_assert(obj_int->value.as_integer == 1, "Integer ot object did not get correct value");

  mu_assert(obj_str->type == tString, "String to object is not of type String");
  mu_assert(bstrcmp(obj_str->value.as_string, bfromcstr("hello")) == 0, "String to object did not get correct value");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_to_object);
  mu_run_test(test_to_string);

  return NULL;
}

RUN_TESTS(all_tests);
