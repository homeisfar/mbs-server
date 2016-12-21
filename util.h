#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>

#include "global.h"
#include "client.h"

#define max(a,b) ((a>b)?a:b)

extern void error_check(ssize_t returncode, char *funcname);
extern void usage();

/* "When compiling, I will need to use -lsocket and -lnsl */

/* Convert a string to a network-byte-ordered numerical IP:

struct sockaddr_in srv;
srv.sin_addr.s_addr = inet_addr(''128.2.15.9'');

To convert an IP address struct to a string:

struct sockaddr_in srv;
char *t = inet_ntoa(srv.sin_addr);
*/
