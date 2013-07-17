#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <mysql/mysql.h>

#include <libxml/parser.h>

#include "define.h"

int query_mysql(const char *);

#endif
