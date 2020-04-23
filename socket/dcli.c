// This program sets up a socket to allow two clients to talk to each other

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/un.h>

#define SOCKETNAME "selectServerSocket"

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

int main(void)
{
  int s, len; // s is this end of the connection and len is length os sockaddr
  int nread, nready;
  int maxfd;
  char buf[1024];
  fd_set fds;
  struct sockaddr_un name;

  // create a socket
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    error("socket");

  // create the address
  memset(&name, 0, sizeof(struct sockaddr_un));

  name.sun_family = AF_UNIX;
  strcpy(name.sun_path, SOCKETNAME);
  len = sizeof(name.sun_family) + strlen(name.sun_path);

  // connect to the server
  if (connect(s, (struct sockaddr *) &name, len) < 0)
    error("connect");

  maxfd = s + 1;
  while(1)
  {
    // set up polling
    FD_ZERO(&fds);
    FD_SET(s, &fds);
    FD_SET(0, &fds);

    // wait for some input
    nready = select(maxfd, &fds, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

    // if either device has input, read it and copy
    if (FD_ISSET(s, &fds))
    {
      nread = recv(s, buf, sizeof(buf), 0);
      if (nread < 1)
      {
        close(s);
        exit(0);
      }
      write(1, buf, nread);
    }

    if (FD_ISSET(0, &fds))
    {
      nread = read(0, buf, sizeof(buf));
      if (nread < 1)
      {
        close(s);
        exit(0);
      }
      send(s, buf, nread, 0);
    }
  }
}
