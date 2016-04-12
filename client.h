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

struct client_node *cli_list;

struct client_node *accept_new_conn(int hostfd);
void add_new_client(struct client_node **cli_list, struct client_node *new_cli_node);
int get_maxfd(struct client_node *cli_list);
int get_read_fdset(fd_set *readfds, int serverfd, struct client_node *cli_list);
