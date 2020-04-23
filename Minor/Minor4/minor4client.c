// Name.....Yafet Kubrom
// EUID.....11334602
// Course...csce3600.001
// ...Minor4 client.....

// compile: gcc minor4client.c -o minor4client
// Usage: ./minor4client hostname port

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <errno.h>
#include <netdb.h>

void error(const char* msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, port, n;
  int nread, nready, maxfd;
  fd_set fds;
  char received[3000];
  char message[255];
  struct sockaddr_in serv_addr;
  struct hostent *server;
  // check for right format input
  if (argc != 3)
  {
    printf("usage: ./minor4client hostname port\n");
    exit(1);
  }

  // create a socket for client
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    error("socket");

  port = atoi(argv[2]); // convert port number into int

  // chech if hostname exist
  if ((server = gethostbyname(argv[1])) == NULL)
  {
    printf("No such host\n");
    exit(EXIT_FAILURE);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));  // clean out garbage value

  // configure socket
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port); // convert to network byte order

  // connect with server socket
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("connect");

    maxfd = sockfd + 1;
    while(1)
    {
      // set up polling
      FD_ZERO(&fds);
      FD_SET(sockfd, &fds);
      FD_SET(0, &fds);

      // wait for some input
      nready = select(maxfd, &fds, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

      char ch[5];

      // if either device has input, read it and copy
      if (FD_ISSET(sockfd, &fds))
      {
        nread = recv(sockfd, received, sizeof(received), 0);
        if (nread < 1)
        {
          close(sockfd);
          exit(0);
        }
        write(1, received, nread);
      }

      if (FD_ISSET(0, &fds))
      {
        nread = read(0, ch, sizeof(ch));
        if (nread < 1)
        {
          close(sockfd);
          exit(0);
        }
        send(sockfd, ch, 2, 0);
      }
    }

  return 0;
}
