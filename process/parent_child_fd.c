#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void printpos(char *msg, int fd)
{
	long int pos;
	if ((pos = lseek(fd, 0L, SEEK_CUR) ) < 0L)
		perror("lseek");

	printf("%s: %ld\n", msg, pos);
}

int main()
{
	int fd;
	pid_t pid;
	char buf[10];
	if ((fd = open("file1", O_RDONLY)) < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	read(fd, buf, 7);	// move pointer
	printpos("Before fork", fd);

	if ((pid = fork()) == 0 )	// child process
	{
		printpos("child before read", fd);
		read(fd, buf, 8);
		printpos("child after read", fd);

	}
	else if (pid > 0)	// parent process
	{
		wait((int *) 0);
		printpos("Parent after wait", fd);
	}

	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}
