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
  Object *str = Object_create_string(S("bar"));
  Object *number = Object_create_integer(42);

  Object *array = String_to_object(S("[42, \"hey\"]"));

  DB_set(db, S("foo"), str);
  DB_set(db, S("number"), number);
  DB_set(db, S("array"), array);

  return NULL;
}

char *test_get()
{
  Object *bar = DB_get(db, S("foo"));
  bstring barstr = Object_to_string(bar);

  mu_assert(eql(barstr, "\"bar\""), "Strings mismatch");

  Object *number = DB_get(db, S("number"));
  bstring numberstr = Object_to_string(number);

  mu_assert(eql(numberstr, "42"), "Integer mismatch");

  Object *ary = DB_get(db, S("array"));
  bstring arystr = Object_to_string(ary);

  mu_assert(eql(arystr, "[42, \"hey\"]"), "Array mismatch");

  return NULL;
}

char *test_apush()
{
  Object *integer = Object_create_integer(999);
  DB_apush(db, S("newarray"), integer);
  return NULL;
}

char *test_apop()
{
  Object *integer = DB_apop(db, S("newarray"));
  bstring integerstr = Object_to_string(integer);

  mu_assert(eql(integerstr, "999"), "Wrong popped value.");
  return NULL;
}

char *test_aat()
{
  Object *integer = DB_aat(db, S("array"), 0);
  bstring integerstr = Object_to_string(integer);

  mu_assert(eql(integerstr, "42"), "Wrong value at index.");
  return NULL;
}

char *test_acount()
{
  Object *count = DB_acount(db, S("array"));
  bstring countstr = Object_to_string(count);
  mu_assert(eql(countstr, "2"), "Wrong array count.");
  return NULL;
}

char *test_hset()
{
  Object *integer1 = Object_create_integer(999);
  Object *integer2 = Object_create_integer(1);

  DB_hset(db, S("hash"), bfromcstr("foo"), integer1);
  DB_hset(db, S("hash"), bfromcstr("bar"), integer2);

  return NULL;
}

char *test_hget()
{
  Object *integer1 = DB_hget(db, S("hash"), bfromcstr("foo"));
  Object *integer2 = DB_hget(db, S("hash"), bfromcstr("bar"));
  bstring integer1str = Object_to_string(integer1);
  bstring integer2str = Object_to_string(integer2);

  mu_assert(eql(integer1str, "999"), "Wrong hash value on creation");
  mu_assert(eql(integer2str, "1"), "Wrong hash value on update");

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
  mu_run_test(test_aat);
  mu_run_test(test_acount);
  mu_run_test(test_hset);
  mu_run_test(test_hget);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
