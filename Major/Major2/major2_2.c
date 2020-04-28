#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <signal.h>
#include <ctype.h>
#include <fcntl.h>

#include "main.h"

static char prompt[256] = "prompt"; // static, global string to hold user-customizable prompt

// interactive mode reads command(s) in a loop
void interactive(char input[], char *aliasNames[], char *aliasCom[]);
void piped(char *input);									   // Yafet
void myhistory(char *input);								   // Yafet
void cd(char *input);										   // Michael
void alias(char *input, char *aliasNames[], char *aliasCom[]); // Michael
//void sigctch(int sig);			// Brandon> tell user what signal was caught in parent
//void sigHPr();          // Brandon> parent signal handler
//void sigHCh();		// Brandon> child signal handler
//void toExit();		// Brandon (we cant name it exit)
void path(char *input);			// Colton
void redirect(char *input);		// Colton
void changePrompt(char *input); // allows user to customize prompt

int digit;			// for history purposes
int digit2;			// for history purposes
char hist[20][100]; // for history purposes

int main(int argc, char *argv[])
{
	char *aliasNames[100]; //(Michael) saves names for aliased commands. size 100 is temp
	char *aliasCom[100];   //(Michael) saves commands that have been aliased. size 100 is temp
	//aliasNames and aliasCom should have the same size
	sigHPr();

	digit = 0;
	digit2 = 0;

	for (int i = 0; i < /*aliasName size*/ 100; i++)
	{
		aliasNames[i] = "";
		aliasCom[i] = "";
	}

	char input[512];
	bzero(input, 512);
	if (argc == 2)
	{
		// take from a file
		// open exesting file (fopen)
		FILE *fp = fopen(argv[1], "r");
		if (!fp)
		{
			printf("Missing: %s not in current directory\n", argv[1]);
			exit(1);
		}
		// read from a file (fgets)
		while (fscanf(fp, "%[^\n]%*c", input) == 1)
		{
			interactive(input, aliasNames, aliasCom);
			bzero(input, 512);
			//(gets(input, 512, fp) != NULL)
		}
		// close a file (fclose)
		fclose(fp);
		// break it down line by line
		// then it bacomes interactive
		//interactive();
	}
	else if (argc == 1)
	{
		//char input[512];  // user input is no more that 512 bytes
		while (1)
		{
			printf("%s> ", prompt);
			scanf("%[^\n]%*c", input);
			if (strlen(input) == 0 || strlen(input) > 512)
			{
				while((getchar()) != '\n');
				continue;
			}
			if (!strcmp(input, "quit"))
			{
				exit(0);
			}
			// take the input and put through interactive
			interactive(input, aliasNames, aliasCom);
			bzero(input, 512);
		}
	}
	else
	{
		printf("usage: ./newshell [file]\n");
		exit(0);
	}

	//if (signal(SIGINT, signalHandler) == SIG_ERR) // just example
	//  perror("signal");
}

void interactive(char input[], char *aliasNames[], char *aliasCom[])
{
	char *tmp[15]; // separate the string by semicolons
	char *cmd[15]; // store command(s) separated by space
	int k = 0; // k will iterate through the array
	int m = 0;
	int runExit = 0;
	int num;   // count words in commands
	int i;	   // used for (for loop)
	pid_t pid; // process id for child process

	int runAlias = 0; //used to detect if an aliased command was entered

	
	//char* temp = malloc(sizeof(char) * 512);
	strcpy(hist[digit++], input);
	digit2++;
	if (digit == 20) digit = 0;

	// split input by semicolons
	input = strtok(input, ";"); // strtok() break up string by semicolons
	while (input != NULL)
	{
		tmp[k] = input;
		input = strtok(NULL, ";"); //iterate througth pointer
		k++;
	}
	tmp[k] = NULL; // k is number of commands

	for (m = 0; m < k; m++)
	{
		char *separated = malloc(sizeof(char) * 512); // stores the current command in tmp that the loop is on
		char *tokInput = malloc(sizeof(char) * 512);  // stores all words for a command separated by single spaces (ex. " ls   -al  " -> "ls -al")

		if (separated == NULL || tokInput == NULL)
		{
			perror("malloc");
			return;
		}
		strcpy(tokInput, "");
		separated = strtok(tmp[m], " ");
		while (separated != NULL)
		{
			strcat(tokInput, separated);
			strcat(tokInput, " ");
			separated = strtok(NULL, " "); // iterate through pointer
		}
		tokInput[strlen(tokInput) - 1] = '\0'; // essentially removes the last space added in the above loop
		tmp[m] = tokInput;
	}

	// so far, commands are separated by ;
	// now we execute each one. assuming there are more than one.

	for (i = 0; i < k; i++)
	{

		
		// if command is pipe, piped(char *input)
		if (strchr(tmp[i], '|') != NULL)
			piped(tmp[i]);
		// else if command is redirect, redirect(char *input)
		else if (strstr(tmp[i], " < ") != NULL || strstr(tmp[i], " > ") != NULL)
			redirect(tmp[i]);
		// else if command is alias, alias(char *input)
		else if (strstr(tmp[i], "alias") != NULL)
			alias(tmp[i], aliasNames, aliasCom);
		// else if command is cd, cd(char *input)
		else if (strstr(tmp[i], "cd") != NULL)
			cd(tmp[i]);
		// else if command is exit, exit(char *input)
		else if (strstr(tmp[i], "exit") != NULL)
			//toExit();
			runExit = 1;
		// else if command is path, path(char *input)
		else if (strstr(tmp[i], "path") != NULL)
			path(tmp[i]);
		// else if command is myhistory, myhistory(char *input)
		else if (strstr(tmp[i], "myhistory") != NULL)
			myhistory(tmp[i]);
		// else if command is changeprompt, changePrompt(char *input)
		else if (strstr(tmp[i], "changeprompt") != NULL)
			changePrompt(tmp[i]);
		// else execute
		else
		{
			for(int h = 0; h < /*aliasNames size*/100; h++)
			{
				if(strcmp(tmp[i],aliasNames[h]) == 0)
				{
					if(strncmp(aliasCom[h], "alias", 5) == 0)
					{
						alias(aliasCom[h], aliasNames, aliasCom);
						runAlias = 1;
					}
					else if(strncmp(aliasCom[h], "cd", 2) == 0)
					{
						cd(aliasCom[h]);
						runAlias = 1;
					}
					else if(strncmp(aliasCom[h], "exit", 4) == 0)
					{
						//toExit();
						runExit = 1;
						runAlias = 1;
					}
					else if(strncmp(aliasCom[h], "path", 4) == 0)
					{
						path(aliasCom[h]);
						runAlias = 1;
					}
					else if(strncmp(aliasNames[h], "myhistory", 9) == 0)
					{
						myhistory(aliasCom[h]);
						runAlias = 1;
					}
					else if(strncmp(aliasNames[h], "changeprompt", 12) == 0)
					{
						changePrompt(aliasCom[h]);
						runAlias = 1;
					}
					else
					{
						tmp[i] = aliasCom[h];
					}
				}
			}
			if(runAlias == 0)
			{
				num = 0;
				tmp[i] = strtok(tmp[i], " "); // separate one command by space
				while (tmp[i] != NULL)
				{
					cmd[num] = tmp[i];
					tmp[i] = strtok(NULL, " ");
					num++;
				}
				cmd[num] = NULL;
				if ((pid = fork()) == 0)
				{
					sigHCh();
					execvp(cmd[0], cmd);
					printf("%s: command not found\n", cmd[0]); //
					exit(0);		// exit so the parent does not execute again
				}
				else if (pid > 0)
					wait((int *)0); // wait on child process to end
				else
					perror("fork");
			}
		}

		bzero((char *)cmd, sizeof(*cmd));
		bzero((char *)tmp, sizeof(*tmp));
	}
	if(runExit == 1)
	{
		toExit();
	}
}

void piped(char *input) // Yafet
{
	int k = 0;	  // increment the three commands
	int d;
	int m = 0;	  // increment a, b, c
	int status;	  // status pipe
	char *tmp[3]; // store array of cmd split by pipe(|)
	pid_t pipe_pid1;
	pid_t pipe_pid2;
	pid_t pipe_pid3;

	// a | b | c
	char *a[4];
	char *b[4];
	char *c[4];

	// split by pipe
	input = strtok(input, "|");
	while (input != NULL)
	{
		tmp[k] = input;
		input = strtok(NULL, "|");
		k++;
	}
	if (k < 2 || k > 3)
	{
		printf("pipe: wrong format. (enter three commands)\n");	// changed
		return;
	}

	// place split commands in a, b, c
	// A.........
	d = 0;
	tmp[m] = strtok(tmp[m], " ");
	while (tmp[m] != NULL)
	{
		a[d] = tmp[m];
		tmp[m] = strtok(NULL, " ");
		d++;
	}
	a[d] = NULL;

	//B...............B
	d = 0;
	m++;
	tmp[m] = strtok(tmp[m], " ");
	while (tmp[m] != NULL)
	{
		b[d] = tmp[m];
		tmp[m] = strtok(NULL, " ");
		d++;
	}
	b[d] = NULL;

	//C................C
	if (k == 3)
	{
		d = 0;
		m++;
		tmp[m] = strtok(tmp[m], " ");
		while (tmp[m] != NULL)
		{
			c[d] = tmp[m];
			tmp[m] = strtok(NULL, " ");
			d++;
		}
		c[d] = NULL;
	}



	// use dup2 to pipe
	// fd1[0] reads
	// fd1[1] writes
	int fd1[2], fd2[2]; // for each pipe
	pipe(fd1);			// reads first then write

	pipe(fd2); // read from fd1 and write stdout

	if ((pipe_pid1 = fork()) == 0) // child process execute first command
	{
		sigHCh();
		dup2(fd1[1], fileno(stdout));
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		// close both ends of all created fd(s) (very important)
		execvp(*a, a); // execute first command
		perror("execvp1");
	}
	else if (pipe_pid1 > 0) // parent process forks again
	{
		if ((pipe_pid2 = fork()) == 0) // second child execute second command
		{
			sigHCh();
			dup2(fd1[0], fileno(stdin));  // duplicate read end of first -> second pipe
			if (k == 3) dup2(fd2[1], fileno(stdout)); // duplicate write end of second -> third pipe
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);

			execvp(*b, b); // execute second command
			perror("execvp2");
		}
		else if (pipe_pid2 > 0 && k == 3) // parent process of second
		{
			if ((pipe_pid3 = fork()) == 0) //third child to execute third command
			{
				sigHCh();
				dup2(fd2[0], fileno(stdin)); // duplicate read end of last command
				close(fd1[0]);
				close(fd1[1]);
				close(fd2[0]);
				close(fd2[1]);

				execvp(*c, c);
				perror("execvp3");
			}
			else if (pipe_pid3 > 0)
			{
				wait((int *)0);
			}
			else
			{
				perror("fork3");
			}
		}
		else if (pipe_pid2 > 0 || k != 3)
		{
			wait((int *)0);
		}

		else
		{
			perror("fork2");
		}
	}
	else
	{
		perror("fork1"); // incase fork does not work
	}

	// only parent get here (unless there is problem)
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);

	for (int i = 0; i < 3; i++)
	{
		wait(&status);
	}
}

void myhistory(char *input) // Yafet
{
	if (digit2 > 20) digit2 = 20;

	int k = 0;
	char *tmp[3];
	if (digit2 > 20)
		digit2 = 20;

	input = strtok(input, " ");
	while (input != NULL)
	{
		tmp[k++] = input;
		input = strtok(NULL, " ");
	}

	if (k == 2 && strstr(tmp[1], "-c"))
	{
		for (int i = 0; i < digit2; i++)
		{
			memset((char *)tmp[i], 0, strlen(tmp[i]));
			digit2 = 0;
			digit = 0;
		}
	}
	else if (k == 3 && strstr(tmp[1], "-e"))
	{
		int t = atoi(tmp[2]);
		char *one[100];
		char *two[100];
		for (int i = 0; i < 100; i++)
		{
			one[i] = "";
			two[i] = "";
		}
		interactive(hist[t - 1], one, two);
	}
	else if (k == 1)
	{

		for (int i = 0; i < digit2; i++)
			printf("%d %s\n", i + 1, hist[i]);
	}
	else
		printf("myhistory: wrong format\n");
}

void cd(char *input) // Michael
{
	int len = strlen(input);
	if ((strcmp(input, "cd") == 0) || (strcmp(input, " cd") == 0))
	{
		chdir(getenv("HOME"));
	}
	else if ((strncmp(input, "cd", 2) == 0))
	{
		char cdLoc[len - 3];
		strcpy(cdLoc, &input[3]);
		if (chdir(cdLoc) != 0)
		{
			perror("cd");
		}
	}
	else if ((strncmp(input, " cd", 3) == 0))
	{
		char cdLoc[len - 4];
		strcpy(cdLoc, &input[4]);
		if (chdir(cdLoc) != 0)
		{
			perror("cd");
		}
	}
}

void alias(char *input, char *aliasNames[], char *aliasCom[]) // Michael
{
	int len = strlen(input);
	int empty = 0;
	if ((strcmp(input, "alias") == 0) || (strcmp(input, " alias") == 0))
	{
		for (int i = 0; i < /*aliasNames size*/ 100; i++)
		{
			if (strcmp(aliasNames[i], "") == 0)
			{
				empty++;
			}
		}

		if (empty == /*aliasNames size*/ 100)
		{
			printf("No commands have been aliased\n");
		}
		else
		{
			for (int i = 0; i < /*aliasNames size*/ 100; i++)
			{
				if (strcmp(aliasNames[i], "") != 0)
				{
					printf("%s has been aliased as %s\n", aliasCom[i], aliasNames[i]);
				}
			}
		}
	}
	else if (strncmp(input, "alias", 5) == 0)
	{
		char alCom[len - 6];
		strcpy(alCom, &input[6]);
		if (strcmp(alCom, "-c") == 0)
		{
			for (int i = 0; i < /*aliasNames size*/ 100; i++)
			{
				aliasNames[i] = "";
				aliasCom[i] = "";
			}
		}
		else if (strncmp(alCom, "-r", 2) == 0)
		{
			len = strlen(alCom);
			char alRm[len - 3];
			strcpy(alRm, &alCom[3]);
			for (int i = 0; i < /*aliasNames size*/ 100; i++)
			{
				if (strcmp(aliasNames[i], alRm) == 0)
				{
					aliasNames[i] = "";
					aliasCom[i] = "";
				}
			}
		}
		else
		{
			int aliased = 0;
			int eqExist = 0;
			int ap1Exist = 0;
			int ap2Exist = 0;

			int nameLen = 0;
			//int comLen = 0;
			len = strlen(alCom);

			//Finds how many characters until equal sign
			for (int i = 0; i < len; i++)
			{
				if (alCom[i] != '=')
				{
					nameLen++;
				}
				else if (alCom[i] == '=')
				{
					eqExist = 1;
					break;
				}
			}
			char alN[nameLen + 1];
			for (int i = 0; i < nameLen; i++)
			{
				alN[i] = alCom[i];
			}
			alN[nameLen] = '\0';

			if (alCom[nameLen + 1] == '\'')
			{
				ap1Exist = 1;
			}

			if (alCom[strlen(alCom) - 1] == '\'')
			{
				ap2Exist = 1;
			}

			char alC[len - (nameLen + 2)];
			int cntr = 0;
			for (int i = nameLen + 2; i < len - 1; i++)
			{
				alC[cntr] = alCom[i];
				cntr++;
			}
			alC[cntr] = '\0';

			//printf("%s\n", alN);
			//rintf("%s\n", alC);

			if (eqExist == 1 && ap1Exist == 1 && ap2Exist == 1)
			{
				for (int i = 0; i < /*aliasNames size*/ 100; i++)
				{
					if (strcmp(aliasNames[i], "") == 0 && aliased == 0)
					{
						//printf("%d\n", i);
						aliasNames[i] = strdup(alN);
						aliasCom[i] = strdup(alC);
						aliased = 1;
						//printf("%s\n", aliasNames[i]);
						//printf("%s\n", aliasCom[i]);
						break;
					}
				}
			}
			else
			{
				printf("To alias a command, use (alias alias_name='command').\n");
				printf("To list all aliased commands, use (alias).\n");
				printf("To remove all defined aliases, use (alias -c).\n");
				printf("To remove a single alias, use (alias -r alias_name.\n");
			}
		}
	}
}

void path(char *input) // Colton
{
	int len = strlen(input);	// stores length of total user input
	char* tok;					// token for use later
	char path[512];				// temp variable to hold path (plus and minus)
	char modPath[512];			// temp variable to hold a modified path (minus)

	if (len == 4) // if input is only "path"
	{
		printf("%s\n", getenv("PATH"));
	}
	else if (len > 4) // if more is specified than just path
	{
		if (input[6] == ' ') // if there is a space after the +/- symbol
		{
			if (input[5] == '+') // if plus command specified
			{
				//printf("ADD\n");
				strcpy(path, getenv("PATH")); // get PATH
				strcat(path, ":");			  // modify PATH
				strcat(path, &input[7]);	  // ...
				setenv("PATH", path, 1);	  // set PATH
				printf("%s\n", getenv("PATH"));
			}
			else if (input[5] == '-') // if minus command specified
			{
				strcpy(path, getenv("PATH"));			// get PATH
				strcpy(modPath, "");					// clear temp variable
				tok = strtok(path, ":");				// tokenize PATH by ':'
				while (tok != NULL)
				{
					if (strcmp(&input[7], tok) != 0)	// if tok != user search
					{
						if (strlen(modPath) != 0)		// if this is not the first entry to temp var
							strcat(modPath, ":");		// ..then add a colon to the left of upcoming entry
						strcat(modPath, tok);			// fill temp var with current token
					}
					tok = strtok(NULL, ":");			// iterate through tokens
				}
				setenv("PATH", modPath, 1);				// set PATH
				printf("%s\n", getenv("PATH"));
			}
			else
			{
				printf("path: illegal option(s) -- %s\n", &input[5]);  	// illegal usage statement
				printf("usage: path [[+|-] [pathname]]\n");				// ...
			}
		}
		else
		{
			printf("path: illegal option(s) -- %s\n", &input[5]);	// illegal usage statement
			printf("usage: path [[+|-] [pathname]]\n");				// illegal usage statement
		}
	}

	bzero(path, 512);		// clear out temp variables
	bzero(modPath, 512);	// ...
}

void redirect(char *input) // Colton
{
	char *tok;					 // stores current token for tokenizing input
	int leftside_chk = 1;		 // if equal to one, then fill left array
	int redirectionleft_chk = 1; // if equal to one, then angle bracket is going left
	char *leftside[30];			 // stores arguments on right side of angle bracket
	char rightside[256];		 // stores the filename on right side of angle bracket
	int l_ind = 0;				 // stores current index in leftside[] array

	pid_t pid; // pid of child process when forking

	if (strchr(input, '>') != NULL)
	{
		redirectionleft_chk = 0; // if right bracket is found, change this value to zero
	}							 //  otherwise the value will stay at one

	tok = strtok(input, " "); // tokenize input
	while (tok != NULL)
	{
		if (strcmp(tok, "<") != 0 && strcmp(tok, ">") != 0) // if current token is not either angle braket
		{
			if (leftside_chk == 1) // if we are currently filling leftside
			{
				leftside[l_ind++] = tok; // ..then fill leftside
			}
			else // else if filling rightside
			{
				if (strlen(rightside) == 0) // if rightside is empty
				{
					strcpy(rightside, tok); // ..then fill it
				}
				else // if already filled, give error
				{
					printf("error: too many args on right side of bracket\n");
					exit(0); // ..then exit
				}
			}
		}
		else
		{
			leftside_chk = 0; // start adding to rightside
		}
		tok = strtok(NULL, " "); // iterate through tokens
	}
	leftside[l_ind] = NULL; // set final to NULL

	if (redirectionleft_chk == 1) // (<) -- if angle bracket is facing left (stdin)
	{
		if ((pid = fork()) == 0) // start a fork
		{
			int ifp = open(rightside, O_RDONLY);			// open input file stream
			dup2(ifp, fileno(stdin));						// change stdin to ifp
			close(ifp);										// close ifp
			execvp(leftside[0], leftside);					// exec leftside commands
			dup2(0, fileno(stdin));							// change stdin back to normal
			printf("%s: command not found\n", leftside[0]); // error message if command failed
			exit(0);										// exit so the parent does not execute again
		}
		else if (pid > 0)
			wait((int *)0); // wait on child process to end
		else
			perror("fork");
	}
	else // (>) -- if angle bracket is facing right (stdout)
	{
		//printf("MADE it here!\n");

		if ((pid = fork()) == 0)
		{
			int orig_stdout = dup(1);																		// saves stdout so we can print "command not found" if needed later
			int ofp = open(rightside, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR); // open output file stream
			dup2(ofp, fileno(stdout));																		// change stdout to ofp
			close(ofp);																						// close ofp
			execvp(leftside[0], leftside);																	// execute leftside commands
			dup2(1, fileno(stdout));																		// restore stdout to normal
			dup2(orig_stdout, 1);																			// restore original stdout
			close(orig_stdout);																				// close orig_stdout
			printf("%s: command not found\n", leftside[0]);													// ensured to print to stdout (and not to file)
			exit(0);																						// exit so the parent does not execute again
		}
		else if (pid > 0)
			wait((int *)0); // wait on child process to end
		else
			perror("fork");
	}
}

void changePrompt(char *input)
{
	int len = strlen(input); // stores length of entire input
	if (len > 499)
	{ // if user input is very long, then give perror
		perror("prompt is too long");
	}
	else if (len == 12)
	{ // if no prompt was specified
		printf("usage: changeprompt <prompt>\n");
	}
	else
	{												 // user input has passed all checks if it makes it into here
		char temp[len - 13];						 // stores the part of user input that holds the new prompt
		strcpy(temp, &input[13]);					 // store prompt into temp var
		strcpy(prompt, temp);						 // save temp var into the static global var, prompt
		printf("prompt changed to: \"%s\"\n", temp); // let user know of changes
	}
}
