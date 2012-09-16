#include "minunit.h"
#include <shitdb/db.h>
#include <shitdb/darray.h>

static DB *db = NULL;

char *test_create()
{
  db = DB_create();
  return NULL;
}

char *test_set()
{
  Object *str = Object_create_string(bfromcstr("bar"));
  Object *number = Object_create_integer(42);

  Object *array = String_to_object(bfromcstr("[42, \"hey\"]"));

  DB_set(db, bfromcstr("foo"), str);
  DB_set(db, bfromcstr("number"), number);
  DB_set(db, bfromcstr("array"), array);

  return NULL;
}

char *test_get()
{
  Object *bar = DB_get(db, bfromcstr("foo"));
  bstring barstr = Object_to_string(bar);

  mu_assert(bstrcmp(barstr, bfromcstr("\"bar\"")) == 0, "Strings mismatch");

  Object *number = DB_get(db, bfromcstr("number"));
  bstring numberstr = Object_to_string(number);

  mu_assert(bstrcmp(numberstr, bfromcstr("42")) == 0, "Integer mismatch");

  Object *ary = DB_get(db, bfromcstr("array"));
  bstring arystr = Object_to_string(ary);

  mu_assert(bstrcmp(arystr, bfromcstr("[42, \"hey\"]")) == 0, "Array mismatch");

  return NULL;
}

char *test_destroy()
{
  DB_destroy(db);
  return NULL;
}


char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_set);
  mu_run_test(test_get);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
