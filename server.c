/**************************************
Main function inside.
Copyright © 2016 Ali Homafar
***************************************/
#include "util.h"

int port = 8000;
int sslport = 8001;
char rootdir[] = "./www";

int main (int argc, char *argv[])
{
  int fd, maxfd, next = 0; //fd for listening. newfd for read/write to client. Next for select.
  ssize_t nbytes, nbytes2, rc = 0;
  int newfd[10]; //Array of accepted descriptors. Ten simultaneous connections.
  socklen_t cli_len;
  char buf[BUFSIZE], response[BUFSIZE];
  struct sockaddr_in srv, cli;

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
  maxfd = fd;
  while (1)
  {
    fd_set readfds;
    FD_ZERO (&readfds);
    FD_SET (fd, &readfds);
    for (int i = 0; i < next; i++)
      FD_SET (newfd[i], &readfds);
    rc = select (maxfd+1, &readfds, 0, 0, 0);
    error_check(rc, "select()");
    if (FD_ISSET(fd, &readfds))
    {
      //TODO: wrap accept so that the struct is created to keep track of all our clients.
      newfd[next] = accept(fd, (struct sockaddr*) &cli, &cli_len);
      // TODO: need a much better algorithm for determining maxfd. ha!

      //////////////////print host name////////////////
      #ifdef DEBUG
      struct hostent *host;
      rc = getpeername(newfd[next], (struct sockaddr *) &cli, &cli_len);
        error_check(rc, "getpeername()");

        if ((host = gethostbyaddr((char *) &cli.sin_addr, //TODO: change this to getnameinfo as recommended by the man page.
        sizeof (cli.sin_addr), AF_INET)) == NULL)
          perror("gethostbyaddr");
        else
          printf("Accepted new client: '%s'\n", host->h_name);
      #endif
      ////////////////end print host name/////////////
      maxfd = max(maxfd, newfd[next]);
      ++next;
    }

    /* The following runs for every descriptor newfd[n] */
    for(int i = 0; i < next; i++)
      if (FD_ISSET(newfd[i], &readfds))
      {
        #ifdef DEBUG
          printf("Servicing fd #%d at index %d.\n", newfd[i], i);
        #endif
        // TODO: put the input and output in sub-while loops.
        // TODO: do not allow users closing connections to shutdown the server!
        // TODO: change read/write to recv/send.
        nbytes = read (newfd[i], buf, BUFSIZE);
        if (errno == ECONNRESET)
        {
          rc = close(newfd[i]);
          printf("Closing fd %d\n", newfd[i]);
          next--;
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
          nbytes2 = write (newfd[i], "bootycall\n", 10);
          error_check(nbytes2, "write()");
          #ifdef DEBUG
            printf("Responding to client.\n");
          #endif
        }


      }
  }

/*
  // Accept is blocking. Listen is not.
  newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);
  error_check(newfd, "accept()");

   while (1)
   {
    nbytes = read(newfd, buf, BUFSIZE);
    error_check(nbytes, "read()");
    strcpy(response, buf);
    error_check(rc, "response sprintf()");
    nbytes2 = write(newfd, response, strlen(response));
    error_check(nbytes2, "write()");
    memset(buf, 0, BUFSIZE);
    memset(response, 0, BUFSIZE);
}
*/

  /* TODO: Obviously i can't just close file descriptors like this now that select() is in */
  // rc = close(fd);
  // error_check(rc, "fd close()");
  // close(newfd);
  // error_check(rc, "newfd close()");
}
