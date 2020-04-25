#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

// interactive mode reads command(s) in a loop
void interactive(char input[]);
void piped(char *input);
void myhistory(char *input);

int main(int argc, char *argv[])
{
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
    while(fscanf(fp,"%[^\n]%*c", input) == 1)
    {
      interactive(input);
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
    char input[512];  // user input is no more that 512 bytes
    while(1)
    {
      printf("prompt> ");
		  scanf("%[^\n]%*c", input);
      if (!strcmp(input, "quit")) exit(0);

      // take the input and put through interactive
      interactive(input);
      bzero(input, 512);
    }
  }
  else
  {
    printf("usage: ./newshell [file]\n");
    exit(0);
  }
}

void interactive(char input[])
{
  char* tmp[15];		// separate the string by semicolons
	char* cmd[15];	 // store command(s) separated by space
	int k = 0;			 // k will iterate through the array
	int num;         // count words in commands
	int i;	         // used for (for loop)
	pid_t pid;       // process id for child process

  // split input by semicolons
  input = strtok(input,";");		// strtok() break up string by space
  while(input != NULL)
  {
    tmp[k] = input;
    input = strtok(NULL, ";");	//iterate througth pointer
    k++;
  }
  tmp[k] = NULL;	// k is number of commands

  // so far, commands are separated by ;
  // now we execute each one. assuming there are more than one.

  for (i = 0; i < k; i++)
  {
    // if command is pipe, piped(char *input)
    // else if command is redirect, redirect(char *input)
    // else if command is cd, cd(char *input)
    // else if command is exit, exit(char *input)
    // else if command is path, path(char *input)
    // else if command is myhistory, myhistory(char *input)
    // else if command is alias, alias(char *input)
    // else execute

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
      //exit(0);		// exit so the parent does not execute again
    }
    else if (pid > 0)
      wait((int *)0 );	// wait on child process to end
    else
      perror("fork");

    bzero((char *)cmd, sizeof(*cmd));
    bzero((char *)tmp, sizeof(*tmp));
  }
}

void piped(char *input)
{

}

void myhistory(char *input)
{

}
