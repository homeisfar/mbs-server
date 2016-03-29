#include "util.h"
// #include "socket.h"

int main ()
{
  int fd, newfd, nbytes, nbytes2, rc; //fd for listening. newfd for read/write to client
  socklen_t cli_len;
  char buf[512], response[512];
  struct sockaddr_in srv, cli;

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1)
  {
    fprintf(stderr, "Fatal Error: creating socket");
    exit(1);
  }

  srv.sin_family = AF_INET;
  srv.sin_port = htons(8000); // TODO: hardcode removal
  srv.sin_addr.s_addr = htonl(INADDR_ANY);

  rc = bind(fd, (struct sockaddr*) &srv, sizeof(srv));
  if ( rc == -1)
  {
    fprintf(stderr, "Fatal Error: binding socket");
    exit(1);
  }

  listen(fd, 5);
  newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);

  //read the client data and write back to it.

  //ERROR CHECKING MISSING. READ NEEDS TO BE IN WHILE LOOP. WRITE NEEDS TO BE IN LOOP. CRLF IS THE DELIMITER.
  nbytes = read(newfd, buf, sizeof(buf));
  sprintf(response, "Take what's coming to ya!\n");
  nbytes2 = write(newfd, response, strlen(response));
  close(fd);
  close(newfd);
}
