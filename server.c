/**************************************
Main function inside.
Copyright © 2016 Ali Homafar
***************************************/
#include "util.h"

int port = 8000;
int sslport = 8001;
char rootdir[] = "./www";
struct client_node *cli_list = NULL;

void initialize_server();

int main (int argc, char *argv[])
{
  #ifdef DEBUG
    printf("Setting up server...\n");
  #endif

  /* Retrieve command line args */
  int ch;
  while ((ch = getopt(argc, argv, "p:s:r:")) != -1) {
    switch (ch) {
      case 'p':
        port = atoi(optarg);
      break;
      case 's':
        sslport = atoi(optarg);
      break;
      case 'r':
        strncpy(rootdir,optarg,sizeof(rootdir)-1);
        rootdir[sizeof(rootdir)-1] = '\0';
      break;
      case '?':
        fprintf(stderr, "Invalid invocation.\n");
      default:
        usage();
        exit(1);
      }
    }
  argc -= optind;
  argv += optind;

  initialize_server();
  return 0;
}

void initialize_server()
{
  int fd, maxfd = 0; //fd for listening. newfd for read/write to client. Next for select.
  ssize_t nbytes, nbytes2, rc = 0;
  char buf[BUFSIZE], response[BUFSIZE];
  struct sockaddr_in srv;

  fd = socket(AF_INET, SOCK_STREAM, 0);
  error_check(fd, "socket()");

  srv.sin_family = AF_INET;
  srv.sin_port = htons(port);
  srv.sin_addr.s_addr = htonl(INADDR_ANY);

  #ifdef DEBUG
    printf("port: %d\n", port);
    printf("sslport: %d\n", sslport);
    printf("servedir: %s\n", rootdir);
  #endif

  rc = bind(fd, (struct sockaddr*) &srv, sizeof(srv));
  error_check(rc, "bind()");

  // Fun fact: the other end can start sending data after listen() and before accept().
  // Three way handshake is done after listen().
  // listen does need to be in a loop or anything like that for further incoming requests
  // because the OS is handling it for us now.
  rc = listen(fd, 5);
  error_check(rc, "listen()");

  //read the client data and write back to it.
  // maxfd = fd;

  fd_set readfds;
  struct client_node *temp;
  while (1)
  {
    maxfd = get_read_fdset(&readfds, fd, cli_list);
    rc = select (maxfd+1, &readfds, 0, 0, 0);
    error_check(rc, "select()");
    if (FD_ISSET(fd, &readfds)){accept_new_conn(fd);}
    /* The following runs for every descriptor in our linked list */
    temp = cli_list;
    while(temp != NULL)
    {
      if (FD_ISSET(temp->data.fd, &readfds))
      {
        #ifdef DEBUG
          printf("Servicing fd #%d.\n", temp->data.fd);
        #endif
        // TODO: put the input and output in sub-while loops.
        // TODO: do not allow users closing connections to shutdown the server!
        // TODO: change read/write to recv/send. MAYBE? CMU prof says just use read.
        nbytes = read (temp->data.fd, buf, BUFSIZE);
        strcpy(response, buf);
        if (errno == ECONNRESET)
        {
          rc = close(temp->data.fd);
          printf("Closing fd %d\n", temp->data.fd);
          error_check (rc, "newfd close()");
        }
        else
        {
          error_check(nbytes, "read()");
          #ifdef DEBUG
            printf("Reading from client.\n");
          #endif
        }
        // Shouldn't do writing here when selecting. but for testing purposes... hrrhrr
        if (errno != ECONNRESET)
        {
          nbytes2 = write(temp->data.fd, response, strlen(response));
          error_check(nbytes2, "write()");
          memset(buf, 0, BUFSIZE);
          memset(response, 0, BUFSIZE);
          #ifdef DEBUG
            printf("Responding to client.\n");
          #endif
        }
      }
    temp = temp->next;
    }
  }
}
