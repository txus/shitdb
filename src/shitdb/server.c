#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <shitdb/db.h>
#include <shitdb/server.h>

int Command_arity(bstring cmd)
{
  if(eql(cmd, "GET")) {
    return 1;
  } else if (eql(cmd, "SET")) {
    return 2;
  } else if (eql(cmd, "DELETE")) {
    return 1;
  } else if (eql(cmd, "APUSH")) {
    return 2;
  } else if (eql(cmd, "APOP")) {
    return 1;
  } else if (eql(cmd, "AAT")) {
    return 2;
  } else if (eql(cmd, "ACOUNT")) {
    return 1;
  } else if (eql(cmd, "HSET")) {
    return 3;
  } else if (eql(cmd, "HGET")) {
    return 2;
  } else if (eql(cmd, "CLEAR")) {
    return 0;
  } else if (eql(cmd, "QUIT")) {
    return 0;
  } else {
    return -1;
  }
}

int Server_execute(DB *db, bstring command, Object *result)
{
  struct bstrList *words = bsplit(command, ' ');

  bstring *ptr = words->entry;

  bstring cmd = *ptr;

  int arity = Command_arity(cmd);

  check(words->qty == arity + 1, "Wrong number of arguments, expected %i, got %i", arity, words->qty - 1);

  ptr++;

  if(eql(cmd, "GET")) {
    Object *ret = DB_get(db, *ptr);
    if (ret) {
      memcpy(result, ret, sizeof(Object));
    } else {
      return 2;
    }
  } else if (eql(cmd, "SET")) {
    bstring key = *ptr;
    ptr++;
    Object *value = String_to_object(*ptr);
    check(value, "Invalid value to SET.");

    DB_set(db, key, value);
  } else if (eql(cmd, "DELETE")) {
    DB_delete(db, *ptr);
  } else if (eql(cmd, "APUSH")) {
    bstring key = *ptr;
    ptr++;
    Object *value = String_to_object(*ptr);
    check(value, "Invalid value to APUSH.");

    DB_apush(db, key, value);
  } else if (eql(cmd, "AAT")) {
    bstring key = *ptr;
    ptr++;
    int index = atoi(bdata(*ptr));

    Object *ret = DB_aat(db, key, index);
    if (ret) {
      memcpy(result, ret, sizeof(Object));
    } else {
      return 2;
    }
  } else if (eql(cmd, "ACOUNT")) {
    bstring key = *ptr;
    Object *ret = DB_acount(db, key);
    memcpy(result, ret, sizeof(Object));
  } else if (eql(cmd, "APOP")) {
    bstring key = *ptr;
    ptr++;

    Object *ret = DB_apop(db, key);
    if (ret) {
      memcpy(result, ret, sizeof(Object));
    } else {
      return 2;
    }
  } else if (eql(cmd, "HSET")) {
    bstring key = *ptr;
    ptr++;
    bstring hkey = *ptr;
    ptr++;
    Object *value = String_to_object(*ptr);
    check(value, "Invalid value to HSET.");

    DB_hset(db, key, hkey, value);
  } else if (eql(cmd, "HGET")) {
    bstring key = *ptr;
    ptr++;
    bstring hkey = *ptr;

    Object *ret = DB_hget(db, key, hkey);
    memcpy(result, ret, sizeof(Object));
  } else if (eql(cmd, "CLEAR")) {
    DB_clear(db);
  } else if (eql(cmd, "QUIT")) {
    return -1;
  }
  return 0;

error:
  return 1;
}

static bstring
chomp(bstring original)
{
  struct bstrList *words = bsplit(original, '\n');
  return *words->entry;
}

/* TODO: Clean up file descriptors when exiting. */
void Server_goodbye()
{
  printf("Goodbye!");
}

void Server_start(DB *db, int port)
{
  // Trap SIGINT and call Server_goodbye()
  (void) signal(SIGINT, Server_goodbye);

  // Declare file descriptors
  int listen_fd, accept_fd;

  int received = 0;

  // Declare server and client addresses
  struct sockaddr_in server, client;

  unsigned int sin_size;

  // Try to open the socket
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  check(listen_fd != -1, "Could not open socket");

  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  // Set our IP address automatically
  server.sin_addr.s_addr = INADDR_ANY;

  // Fil the rest of the struct with zeros
  bzero(&(server.sin_zero), 8);

  if (bind(listen_fd, (struct sockaddr* )&server, sizeof(server)) == -1) {
    debug("Could not bind the socket.");
    goto error;
  }

  debug("Server listening at port %i...", port);

  listen(listen_fd, 5);

  int rc = 0;

  again:
  while(1) {
    sin_size = sizeof(struct sockaddr_in);

    // Accept incoming connection
    if ((accept_fd = accept(listen_fd,
                            (struct sockaddr *)&client,
                            &sin_size)) == -1) {
      debug("Could not accept incoming connection.");
      goto error;
    }

    debug("Incoming connection from %s", inet_ntoa(client.sin_addr));

    // Receive the message
    char buf[256];
    while((received = recv(accept_fd, buf, 255, 0))) {
      Object *result = Object_allocate();
      rc = Server_execute(db, chomp(S(buf)), result);
      if(rc == 0) {
        if (result->type != tNil) {
          bstring ret = Object_to_string(result);
          bconchar(ret, '\n');
          send(accept_fd, bdata(ret), blength(ret), 0);
        } else {
          send(accept_fd, "OK\n", 4, 0);
        }
      } else if (rc == 1) {
        send(accept_fd, "ERROR\n", 7, 0);
      } else if (rc == 2) {
        send(accept_fd, "NOTFOUND\n", 10, 0);
      } else if (rc == -1) {
        send(accept_fd, "BYE\n", 5, 0);
        // Close the connection
        close(accept_fd);
        goto again;
      }
    };
  }

error:

  debug("Bye!");
}
