#pragma once
/******************
The clientdata struct contains data about each new client who has connected to our server
*******************/
#include "util.h"

struct client_data
{
  int fd;
  struct sockaddr_in clisock;
  socklen_t clisock_len;
};

/* Use a linked list to manage all of our connected clients.
   After calling select(), we will iterate through the list and test
   if a socket has active data. If so, read out that data. */
struct client_node
{
  struct client_node *next;
  struct client_data data;
};
