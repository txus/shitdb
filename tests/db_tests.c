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

char *test_apush()
{
  Object *number = Object_create_integer(999);
  DB_apush(db, bfromcstr("array"), number);
  return NULL;
}

char *test_apop()
{
  Object *integer = DB_apop(db, bfromcstr("array"));
  bstring integerstr = Object_to_string(integer);

  mu_assert(bstrcmp(integerstr, bfromcstr("999")) == 0, "Wrong popped value.");
  return NULL;
}

char *test_acount()
{
  Object *count = DB_acount(db, bfromcstr("array"));
  bstring countstr = Object_to_string(count);
  mu_assert(bstrcmp(countstr, bfromcstr("2")) == 0, "Wrong array count.");
  return NULL;
}

char *test_hset()
{
  Object *integer1 = Object_create_integer(999);
  Object *integer2 = Object_create_integer(1);

  DB_hset(db, bfromcstr("hash"), bfromcstr("foo"), integer1);
  DB_hset(db, bfromcstr("hash"), bfromcstr("bar"), integer2);

  return NULL;
}

char *test_hget()
{
  Object *integer1 = DB_hget(db, bfromcstr("hash"), bfromcstr("foo"));
  Object *integer2 = DB_hget(db, bfromcstr("hash"), bfromcstr("bar"));
  bstring integer1str = Object_to_string(integer1);
  bstring integer2str = Object_to_string(integer2);

  mu_assert(bstrcmp(integer1str, bfromcstr("999")) == 0, "Wrong hash value on creation");
  mu_assert(bstrcmp(integer2str, bfromcstr("1")) == 0, "Wrong hash value on update");

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
  mu_run_test(test_apush);
  mu_run_test(test_apop);
  mu_run_test(test_acount);
  mu_run_test(test_hset);
  mu_run_test(test_hget);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
