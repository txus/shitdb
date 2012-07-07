#include "minunit.h"
#include <shitdb/db.h>

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

  DB_set(db, bfromcstr("foo"), str);
  DB_set(db, bfromcstr("number"), number);

  return NULL;
}

char *test_get()
{
  Object *bar = DB_get(db, bfromcstr("foo"));

  mu_assert(bar->type == tString, "Got non-string");
  mu_assert(bstrcmp(bar->value.as_string, bfromcstr("bar")) == 0, "Strings mismatch");

  Object *number = DB_get(db, bfromcstr("number"));

  mu_assert(number->type == tInteger, "Got non-number");
  mu_assert(number->value.as_integer == 42, "Numbers mismatch");

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
