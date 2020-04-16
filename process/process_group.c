#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;

	if ((pid = fork()) == 0)	// child process
	{
		//setpgid(getpid(),getppid());
		printf("Child: pid:%d ppid:%d pgid=%d\n", getpid(), getppid(), getpgrp());
	}

	else if (pid > 0) 	// parent process
	{
		//setpgid(pid, pid);
		printf("Parent: pid:%d ppid:%d pgid=%d\n", getpid(), getppid(), getpgrp());

		// print about child from parent
		printf("Parent: Child's pid:%d pgid=%d\n", pid, getpgid(pid));
		// getpgrp returns group id of current process while getpgid returns of specified process
	}
	else
	{
		perror("fork");
		exit(1);
	}

	return 0;
}
