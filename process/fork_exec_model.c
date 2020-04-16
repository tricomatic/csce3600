/* C program to illustrate use of fork()
  & exec() system calls for
  process creation */

#include <stdio.h>
#include <sys/types.h>		// fork()
#include <sys/wait.h>		// wait() and waitpid()
#include <unistd.h>		// exec(), getpid()
#include <errno.h>		// perror prints reason
#include <stdlib.h>		// fork()

int main()
{
	pid_t pid;
	int ret=1, status, i;
	pid = fork();

	if (pid == 0)	// child process
	{
		printf("child process(%u)....its parent(%u)\n", getpid(), getppid());

		char *cmd[] = {"who", "ls", "date"};
		printf("0=who 1=ls 2=date: ");
		scanf("%d", &i);

		execlp(cmd[i], cmd[i], (char *)0);
		printf("Child: execlp failed\n");
	}
	else if (pid > 0)
	{
		printf("parent process(%u).... its parent(%u)\n", getpid(), getppid());
		if (waitpid(pid, &status, 0) > 0)
		{
			if (WIFEXITED(status) && !WEXITSTATUS(status))
			{
				// WIFEXITED RETURN TRUE WHEN EXIT SUCCUSSFUL AND WEXITSTATUS IS FALSE IF STATUS HAD NO ISSUE
				printf("EXECUTATION SUCCESSFUL\n");
			}
			else if (WIFEXITED(status) && WEXITSTATUS(status))
			{
				if (WEXITSTATUS(status) == 127)
					printf("Parent: execlp failed\n");
				else
					printf("normal termination, non zero status\n");
			}
			else
				printf("non-normal termination\n");
		}
		else
			perror("waitpid");
	}
	return 0;
}
