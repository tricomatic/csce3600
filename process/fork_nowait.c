#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid;	// could be int
	int i;
	pid = fork();
	printf("PID=%d\n", pid);

	if (pid > 0)	// parent process
	{
		for (i=0; i < 100; i++)
			printf("\t\t\tParent %d %d\n", getpid(), i);
	}

	else if (pid == 0)
	{
		for (i=0; i < 100; i++)
			printf("Child %d %d\n", getpid(), i);
	}

	else
	{
		printf("Error: Child could not be created");
	}

	return 0;
}
