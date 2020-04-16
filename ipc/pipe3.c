// model the command-line command: ps -ef | wc using pipes

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
enum { READ, WRITE };

int main()
{
	int fd[2];
	if (pipe(fd) == -1)
	{
		perror("Pipe");
		exit(1);
	}

	switch (fork())
	{
		case -1:
			perror("Fork");
			exit(2);
		case 0:
			dup2(fd[WRITE], fileno(stdout));
			close(fd[READ]);
			close(fd[WRITE]);
			execl("/bin/ps", "ps", "-ef", (char *)0 );
			exit(3);
		default:
			dup2(fd[READ], fileno(stdin));
			close(fd[READ]);
			close(fd[WRITE]);
			execl("/usr/bin/wc", "wc", (char *)0 );
			exit(4);
	}
	return 0;
}
