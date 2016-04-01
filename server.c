#include "util.h"

#define BUFSIZE 2048

int main ()
{
  int fd, maxfd, next = 0; //fd for listening. newfd for read/write to client. Next for select.
  ssize_t nbytes, nbytes2, rc = 0;
  int newfd[10]; //Array of accepted descriptors. Ten simultaneous connections.
  socklen_t cli_len;
  char buf[BUFSIZE], response[BUFSIZE];
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
      newfd[next] = accept(fd, (struct sockaddr*) &cli, &cli_len);
      // TODO: need a much better algorithm for termining maxfd. ha!
      maxfd = max(maxfd, newfd[next]);
      ++next;
    }

    /* The following runs for every descriptor newfd[n] */
    for(int i = 0; i < next; i++)
      if (FD_ISSET(newfd[i], &readfds))
      {
        // TODO: put the input and output in sub-while loops.
        nbytes = read (newfd[i], buf, BUFSIZE);
        error_check(nbytes, "read()");
        nbytes2 = write (newfd[i], "bootycall\n", 10);
        error_check(nbytes2, "write()");
      }
  }

  //ERROR CHECKING MISSING. READ NEEDS TO BE IN WHILE LOOP. WRITE NEEDS TO BE IN LOOP.
  //CRLF IS THE DELIMITER. WHAT 2 DO IF WE GET NON-DELIMITED DATA?

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
  rc = close(fd);
  error_check(rc, "fd close()");
  close(newfd);
  error_check(rc, "newfd close()");
}
