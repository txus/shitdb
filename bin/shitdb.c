#include <shitdb/db.h>
#include <shitdb/server.h>

#define DEFAULT_PORT 4999

static int
resolve_port(int argc, char **argv)
{
  int port = 0;
  if(argc == 2 && (port = atoi(argv[1]))) {
    return port;
  } else {
    return DEFAULT_PORT;
  }
}

int main(int argc, char **argv)
{
  DB *db = DB_create();

  int port = resolve_port(argc, argv);

  Server_start(db, port);

  return 0;
}
