// util.c
#include "util.h"

  /* Do I really like doing this? Is error checking worth the compute time
     of creating a new stack frame for the function calls?
     Since this isn't a high performance system with jillions of requests... yes! */
void error_check(ssize_t returncode, char *funcname)
{
  if (returncode < 0)
  {
    int errsave = errno;
    char *errcode = strerror(errsave);
    fprintf(stderr, "Fatal Error: %s. Error #: %d. %s\n", funcname, errsave, errcode);
    exit(1);
  }
}

void usage()
{
  fprintf(stderr, "Invocation: ./mbs-server -p <port> -s <sslport> -r </path/to/serve/dir\n");
  fprintf(stderr, "./mbs-server alone will give 8000, 8001, and ./www, respectively\n");
}
