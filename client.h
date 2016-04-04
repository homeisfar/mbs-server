#pragma once
/******************
The clientdata struct contains data about each new client who has connected to our server
*******************/
struct clientdata
{
  int fd;
  struct sockaddr_in clisock;
  socklen_t clisock_len;
};
