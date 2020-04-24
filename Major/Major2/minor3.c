
#include <stdio.h>
#include <stdlib.h>	// fork()
#include <unistd.h>	// execvp(), getpid(), getppid()
#include <string.h>
#include <errno.h>	//perror() prints reason for error
#include <sys/wait.h>	// wait() and waitpid();

int main()
{
	char input[512];			// input stores user prompted
	char* tmp[15];		// separate the string by ;
	char* cmd[15];		// store command(s) separated by space
	int k = 0;			// k will iterate through the array
	int num;
	char *ptr;
	int i;	// used for (for loop)
	pid_t pid;

	while(1)
	{
		printf("prompt> ");
		scanf("%[^\n]%*c", input);		// fgets() stores string with spaces into input
		ptr = strtok(input, "\n");	// strtok() removes new line
		if(!strcmp(ptr,"quit")) exit(0);

		ptr = strtok(ptr,";");		// strtok() break up string by space
		while(ptr != NULL)
		{
			tmp[k] = ptr;
			ptr = strtok(NULL, ";");	//iterate througth pointer
			k++;
		}
		tmp[k] = NULL;	// k is number of commands

		// so far, commands are separated by ;
		// now we execute each one. assuming there are more than one.

		for (i = 0; i < k; i++)
		{
			num = 0;
			tmp[i] = strtok(tmp[i], " ");	// separate one command by space
			while (tmp[i] != NULL)
			{
				cmd[num] = tmp[i];
				tmp[i] = strtok(NULL, " ");
				num++;
			}
			cmd[num] = NULL;
			if((pid = fork()) == 0)
			{
				execvp(cmd[0], cmd);
				printf("%s: command not found\n", tmp[i]);	//
				exit(0);		// exit so the parent does not execute again
			}
			else if (pid > 0)
				wait((int *)0 );	// wait on child process to end
			else
				perror("fork");
		}
		k = 0;
		bzero((char *)cmd, sizeof(*cmd));
	}
}
