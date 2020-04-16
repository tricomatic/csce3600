// implementation of socketpair
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

int main()
{
  int fd[2];  // pair of socket descriptors
  char buf;   // for data exchange
  char letter;

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1)
  {
    perror("sockpair");
    exit(1);
  }

  if (!fork())  // if (fork() == 0) child process
  {
    read(fd[1], &buf, 1);
    printf("child: read '%c'\n", buf);
    buf = toupper(buf); // make it uppercase
    write(fd[1], &buf, 1);
    printf("Child: sent '%c'\n", buf);
  }
  else  // parent process
  {
    printf("Enter letter: ");
    scanf("%c", &letter);
    write(fd[0], &letter, 1);
    read(fd[0], &buf, 1);
    printf("parent: read '%c'\n", buf);
    wait(NULL); // wait for child to finish execute
  }

  return 0;
}
