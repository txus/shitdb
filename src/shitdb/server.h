#ifndef _server_
#define _server_

#include <shitdb/db.h>
#include <shitdb/bstrlib.h>

int Command_arity(bstring cmd);
void Server_goodbye();
void Server_start(DB *db, int port);
int Server_execute(DB *db, bstring command);

#endif
