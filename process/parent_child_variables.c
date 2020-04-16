#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int glbvar = 6;

int main()
{
	int locvar = 88;
	pid_t pid;
	printf("Before fork\n");
	if ( (pid = fork()) == 0 )	// child process
	{
		printf("Child process = %d\n", getpid());
		printf("Parent process = %d\n", getppid());
		glbvar++;
		locvar++;
	}
	else if (pid > 0)  // parent process
	{
		sleep(2);
	}
	else
	{
		perror("fork error");
	}

	printf("pid=%d, glbvar=%d, locvar=%d\n", getpid(), glbvar, locvar);

	return 0;
}
