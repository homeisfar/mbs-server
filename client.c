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
  struct client_node *cli_node = init_client_struct();
  //TODO: Do I need to cast?
  cli_node->data.fd = accept(hostfd, (struct sockaddr*)&cli_node->data.clisock, &cli_node->data.clisock_len);

  if (cli_node->data.fd == -1)
  {
    printf("A new connection failed.\n");
    return NULL;
  } else {
  /* put client into linked list */
    /* Pass the address of the linked list, which itself is a pointer.
       Have to double dereference. */
    add_new_client(&cli_list, cli_node);
    //////////////////print host name////////////////
    #ifdef DEBUG
    struct hostent *host;
    int rc = getpeername(cli_node->data.fd, (struct sockaddr *) &cli_node->data.clisock, &cli_node->data.clisock_len);
      error_check(rc, "getpeername()");
      if ((host = gethostbyaddr((char *) &cli_node->data.clisock.sin_addr, //TODO: change this to getnameinfo as recommended by the man page.
      sizeof (&cli_node->data.clisock.sin_addr), AF_INET)) == NULL)
        perror("gethostbyaddr");
      else
        printf("Accepted new client: '%s'\n", host->h_name);
    #endif
    ////////////////end print host name/////////////
  }
  return cli_node;
}

//TODO:does clilist need to be a double pointer?
void add_new_client(struct client_node **cli_list, struct client_node *new_cli_node)
{
  /* we are always putting the new node at the end, even if it's the first node */
  new_cli_node->next = NULL;
  struct client_node *temp = NULL;
  if(*cli_list == NULL)
  {
    *cli_list = new_cli_node;
  }
  else
  {
    temp = *cli_list;
    while (temp->next != NULL)
    {
      temp = temp->next;
    }
    temp->next = new_cli_node;
  }
}

/* I am reasonably sure that maxfd shouldn't include the server fd */
/* on second thought, i already wrote code that assumed it should. Why did I do that? Was that right!? Argggh */
/* To be used when a node is removed */

//FINAL THOUGHT: you need to know the serverfd and if has been set b/c then you know to accept a new connection
int get_maxfd(struct client_node *cli_list)
{
  int maxfd = 0;
  while(cli_list != NULL)
  {
  maxfd = max(maxfd, cli_list->data.fd);
  cli_list = cli_list->next;
  }
  return maxfd;
}

/* While going through our file descriptor set, may as well calculate maxfd */
int get_read_fdset(fd_set *readfds, int serverfd, struct client_node *cli_list)
{
  int maxfd = serverfd;
  FD_ZERO(readfds);
  FD_SET(serverfd, readfds);
  while(cli_list != NULL)
  {
    maxfd = max(maxfd, cli_list->data.fd);
    FD_SET(cli_list->data.fd, readfds);
    cli_list = cli_list->next;
  }
  return maxfd;
}
