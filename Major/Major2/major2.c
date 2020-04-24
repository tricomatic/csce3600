/*	Michael Rakestraw
	CSCE 3600 SEC 001
	3/28/2020

	Continuously prompts user to enter a command, and if the command is valid, forks a child and the child executes the command, then exits.
	If the command is not valid, the child tells the user it is not valid, then exits. If "quit" is entered, exit program.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	//infinite loop
	while(1)
	{
		char command[150];//character array(string) to hold command entered by user
		int cntr = 0; //counter variable

		//print out prompt & save command
		printf("minor3> ");
		scanf("%[^\n]%*c", command);//reads in a string including spaces

		//tests if user entered "quit" & exits program if so
		if(strcmp(command, "quit") == 0)
		{
			exit(0);//exits program
		}

		//divides command into arguments using strtok();
		char* arg; //holds an individual argument
		char *args[150];//array of all arguments

		//divides command string into individual arguments
		arg = strtok(command, " ");
		while(arg != NULL)
		{
			args[cntr] = arg;
			cntr ++;

			arg = strtok(NULL, " ");
		}
		args[cntr] = NULL;//adds NULL to end of args array

		//forks and executes arguments
		pid_t pID = fork();
		//fork failed
		if(pID == -1)
		{
			printf("Could not fork.\n");
		}
		//child process
		else if(pID == 0)
		{
			//if command cannot be executed, outputs "command: command not found, but makes sure not to output "quit: command not found" if quit is entered
			//Otherwise, executes command
			if(execvp(args[0], args) == -1 && strcmp(command, "quit") != 0)
			{
				printf("%s: command not found\n", command);
			}
			//exits child process when done
			exit(0);
		}
		//parent process
		else
		{
			//parent waits for child to finish
			wait(NULL);
		}
	}
	return 0;
}
