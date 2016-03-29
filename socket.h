//This is all written in #include <netinet/in.h>

struct in_addr {
  u_long s_addr;
};

struct sockaddr_in {
  u_char sin_family; /* Address family */
  u_short sin_port; /* Port # */
  struct in_addr sin_addr; /* IP addr */
  char sin_zero[8]; /* unusued */
};
