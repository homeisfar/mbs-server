#include "util.h"

int main ()
{
  int fd, newfd, next = 0; //fd for listening. newfd for read/write to client. Next for select.
  ssize_t nbytes, nbytes2 = 0;
  // int newfd[10]; //Array of accepted descriptors
  int rc = 0;
  socklen_t cli_len;
  char buf[512], response[512];
  struct sockaddr_in srv, cli;

  fd = socket(AF_INET, SOCK_STREAM, 0);
  error_check(fd, "socket()");

  srv.sin_family = AF_INET;
  srv.sin_port = htons(8000); // TODO: hardcode removal
  srv.sin_addr.s_addr = htonl(INADDR_ANY);

  // There is almost a logic flaw with reusing rc, but if it's ever -1 then we exit anyway.
  rc = bind(fd, (struct sockaddr*) &srv, sizeof(srv));
  error_check(rc, "bind()");

  // Fun fact: the other end can start sending data after listen() and before accept().
  // Three way handshake is done after listen().
  rc = listen(fd, 5);
  error_check(rc, "listen()");

  // Accept is blocking. Listen is not.
  newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);
  error_check(newfd, "accept()");

  //read the client data and write back to it.
  /*
  while (1)
  {
    fd_set readfds;
    FD_ZERO (&readfds);
    FD_SET (fd, &readfds);
    // missing code belongs on this line.
    rc = select (maxfd + 1, &readfds, 0, 0, 0);
    if (rc == - 1)
    {
      fprintf(stderr, "Fatal Error: select");
      exit(1);
    }
    if (FD_ISSET(fd, &readfds))
    {
      newfd[next++] = accept(fd, (struct sockaddr*) &cli, &cli_len);
    }
    if (FD_ISSET(newfd[i], &readfds))
    {
      read (newfd[i], buf, sizeof(buf));
    }
  }
  */
  //ERROR CHECKING MISSING. READ NEEDS TO BE IN WHILE LOOP. WRITE NEEDS TO BE IN LOOP.
  //CRLF IS THE DELIMITER. WHAT 2 DO IF WE GET NON-DELIMITED DATA?
  nbytes = read(newfd, buf, sizeof(buf));
  error_check(nbytes, "read()");
  sprintf(response, "Take what's coming to ya!\n");
  nbytes2 = write(newfd, response, strlen(response));
  error_check(nbytes2, "write()");
  close(fd);
  close(newfd);
}

/* select stuff:
ex use:
int select (int maxfds, fd_set *readfds, *writefds, *exceptfds, struct timeval *timeout);

void
FD_CLR(fd, fd_set *fdset);

void
FD_COPY(fd_set *fdset_orig, fd_set *fdset_copy);

int
FD_ISSET(fd, fd_set *fdset);

void
FD_SET(fd, fd_set *fdset);

void
FD_ZERO(fd_set *fdset);
*/
