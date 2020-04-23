// This program sets up a socket to allow two clients to talk to each other

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/un.h>
#include <errno.h>

#define SOCKETNAME "selectServerSocket"

void error(const char *msg)
{
  perror(msg);
  exit(1);
}


int main(void)
{
  char buf[1024], msg[1024];  // buffer for message to others
  int s, ns, ns2;   // listen socket, first connection, second connections
  int len, maxfd;
  int nread, nready;
  struct sockaddr_un name;
  fd_set fds;   // set of file descriptors to poll

  unlink(SOCKETNAME);

  // create the socket
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    error("socket");

  // create the address for server
  memset(&name, 0, sizeof(struct sockaddr_un));

  name.sun_family = AF_UNIX;
  strcpy(name.sun_path, SOCKETNAME);
  len = sizeof(name.sun_family) + strlen(name.sun_path);

  // bind socket to address
  if (bind(s, (struct sockaddr *) &name, len) < 0)
    error("bind");

  // listen for connection
  if (listen(s, 5) < 0)
    perror("listen");

  // accept connection
  if ((ns = accept(s, (struct sockaddr *)&name, &len)) < 0)
    error("accept");

  // accept second connection
  if ((ns2 = accept(s, (struct sockaddr *)&name, &len)) < 0)
    error("accept");

  maxfd = (ns > ns2 ? ns : ns2) + 1;

  while(1)
  {
    // set up polling using select
    FD_ZERO(&fds);
    FD_SET(ns, &fds);
    FD_SET(ns2, &fds);

    // wait for some input
    nready = select(maxfd, &fds, (fd_set *)0, (fd_set *) 0, (struct timeval *) 0);

    // if either descriptor has some input, read it and copy to the other
    if (FD_ISSET(ns, &fds))
    {
      nread = recv(ns, buf, sizeof(buf), 0);
      if (nread < 1)
      {
        close(ns);
        close(ns2);
        exit(0);
      }
      strcpy(msg, "CLIENT 1: ");
      strcat(msg, buf);
      send(ns2, msg, nread + 10, 0);
    }

    if (FD_ISSET(ns2, &fds))
    {
      nread = recv(ns2, buf, sizeof(buf), 0);
      if (nread < 1)
      {
        close(ns);
        close(ns2);
        exit(0);
      }
      strcpy(msg, "CLIENT 2: ");
      strcat(msg, buf);
      send(ns, msg, nread + 10, 0);
    }
  }
}
