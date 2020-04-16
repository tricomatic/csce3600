//NAME...Yafet Kubrom
//EUID...11334602
//DATE...3/26/2020
//Simple command-line interpreter for Linux Kernel

#include <stdio.h>
#include <stdlib.h>	// fork()
#include <unistd.h>	// execvp(), getpid(), getppid()
#include <string.h>
#include <errno.h>	//perror() prints reason for error
#include <sys/wait.h>	// wait() and waitpid();

int main()
{
	char input[20];			// input stores user prompted
	char* command[5];		// command stores the parsed command
	int k = 0;			// k will iterate through the array
	char *ptr;
	pid_t pid;
	while(1)
	{
		printf("minor3> ");
		fgets(input,20,stdin);		// fgets() stores string with spaces into input
		ptr = strtok(input, "\n");	// strtok() removes new line
		if(!strcmp(ptr,"quit")) exit(0);

		ptr = strtok(ptr," ");		// strtok() break up string by space
		while(ptr != NULL)
		{
			command[k] = ptr;
			ptr = strtok(NULL, " ");	//iterate througth pointer
			k++;
		}
		command[k] = NULL;
		pid = fork();
		if(pid == 0)		// child process
		{
			execvp(command[0], command);
			printf("%s: command not found\n",input);
			exit(0);	// exit so the parent does not execute again
		}
		else if (pid > 0)
		{
			wait( (int *)0 );	// wait on child process to end
		}
		else
		{
			perror("fork");
		}
		k = 0;
	}
}
