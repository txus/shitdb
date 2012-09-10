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
  if(bstrcmp(cmd, bfromcstr("GET")) == 0) {
    return 1;
  } else if (bstrcmp(cmd, bfromcstr("SET")) == 0) {
    return 2;
  } else {
    return -1;
  }
}

int Server_execute(DB *db, bstring command)
{
  struct bstrList *words = bsplit(command, ' ');

  bstring *ptr = words->entry;

  bstring cmd = *ptr;

  check(words->qty != Command_arity(cmd) + 1, "Wrong number of arguments");

  Object *result = NULL;

  ptr++;
  if(bstrcmp(command, bfromcstr("GET"))) {
    result = DB_get(db, *ptr++);
  } else if (bstrcmp(command, bfromcstr("SET"))) {
    DB_set(db, *ptr++, String_to_object(*ptr++));
  }
  return 0;

error:
  return 1;
}

/* TODO: Clean up file descriptors when exiting. */
void Server_goodbye()
{
  printf("Goodbye!");
}

void Server_start(DB *db, int port)
{
  // Trap SIGINT and call Server_goodbye()
  signal(SIGINT, Server_goodbye);

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

  listen(listen_fd, 5);

  int rc = 0;

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

    // Receive the probe
    char buf[256];
    received = recv(accept_fd, buf, 255, 0);
    debug("Message received: %s", buf);
    if (received == 1) {
      rc = Server_execute(db, bfromcstr(buf));
      if(rc == 0) {
        send(accept_fd, "OK", 3, 0);
      } else {
        send(accept_fd, "ERROR", 6, 0);
      }
    }

    // Close the connection
    close(accept_fd);
  }

error:

  debug("Bye!");
}
