#include <shitdb/db.h>
#include <shitdb/server.h>
#define DEFAULT_PORT 4999

int main(int argc, char **argv)
{
  DB *db = DB_create();

  Server_start(db, DEFAULT_PORT);

  return 0;
}
