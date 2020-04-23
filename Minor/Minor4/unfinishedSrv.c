// Name.....Yafet Kubrom
// EUID.....11334602
// Course...csce3600.001
// ...Minor4 server.....

// compile: gcc minor4server.c -o minor4server
// Usage: ./minor4server port

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

void error(const char* msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, newsock1, newsock2, port, n;
  socklen_t clilen1, clilen2;
  char str[256];
  char message[] = "Can you see this message?  ";
  struct sockaddr_in serv_addr, cli_addr1, cli_addr2;

  // check if right format is entered
  if (argc != 2)
  {
    printf("format: ./minor4server port\n");
    exit(1);
  }

  // create a socket for server
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    error("socket");

  // zero out garbage numbers from previos program
  bzero((char *) &serv_addr, sizeof(serv_addr));
  port = atoi(argv[1]);  // convert port string to int

  // configure socket before bind
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port); // convert to network byte order

  int on = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  // bind a name to socket (reserve a port)
  if (bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("bind");

  // listen for connection
  listen(sockfd, 3);

  int i;

  for(;;)
  {
    printf("Network Server Starting..\n");
    printf("Waiting on clients\n");

    // accept a connection and return new file descriptor
    clilen1 = sizeof(cli_addr1);
    if ((newsock1 = accept(sockfd, (struct sockaddr *)&cli_addr1, &clilen1)) == -1)
      error("accept");

    printf("X<Connected\n");
    printf("X>You are player X. Waiting on player O to connect\n");

    clilen2 = sizeof(cli_addr2);
    if ((newsock2 = accept(sockfd, (struct sockaddr *)&cli_addr2, &clilen2)) == -1)
      error("accept");

    printf("O<Connected\n");
    printf("O>You are player O\n");

    bzero(str, 256);   // clean out garbage values in buffer

    i = 1;

    while(strcmp(str, "yes"))
    {
      if(send(newsock1, message, sizeof(message), 0) == -1)
        error("send");
      n = recv(newsock1, str, 255, 0);
      str[n] = '\0';
      printf("attempt %d\n", i++);
    }

    bzero(str, 256);
    i = 1;

    while(strcmp(str, "yes"))
    {
      if(send(newsock2, message, sizeof(message), 0) == -1)
        error("send");
      n = recv(newsock2, str, 255, 0);
      str[n] = '\0';
      printf("attempt %d\n", i++);
    }
    close(newsock1);
    close(newsock2);
  }
  close(sockfd);

  return 0;

}
