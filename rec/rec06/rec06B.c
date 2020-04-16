//Name....Yafet Kubrom
//EUID....11334602
//DATE....3/6/2020
//..Orphan Process..

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char *name;
	pid_t pid;
	pid = fork();

	if(pid == 0)		// child process
	{
		printf("child: %d started\n", getpid());	// getpid system call returns current process id
		printf("child: parent = %d\n", getppid());	// getppid system call returns parent process id
		printf("child: ...\n");
		sleep (25);
		printf("child: woke up\n");
	}

	else if(pid > 0)	// parent process
	{
		printf("parent: %d started\n", getpid());
		printf("parent: parent = %d\n", getppid());
	}

	else
	{
		perror("fork error");
		exit(1);
	}

	name = (pid == 0) ? "child" : "parent";	// ternary operator
	printf("%s:terminating...\n", name);
	exit(0);
 }
