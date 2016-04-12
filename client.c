/* client handling code */

#include "client.h"

struct client_node *init_client_struct()
{
  /* Cast the result of malloc to our struct's pointer. Malloc returns void * otherwise */
  struct client_node *newnode = (struct client_node *)malloc(sizeof(struct client_node));
  newnode->data.clisock_len = sizeof(struct sockaddr_in);
  return newnode;
}

struct client_node *accept_new_conn(int hostfd)
{
  struct client_node *clientnode = init_client_struct();
}
