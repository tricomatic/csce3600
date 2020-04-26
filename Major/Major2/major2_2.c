#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

#include <errno.h>


// interactive mode reads command(s) in a loop
void interactive(char input[]);
void piped(char *input);      // Yafet
void myhistory(char *input);  // Yafet
void cd (char *input);        // Michael
void alias(char *input);      // Michael
//void signalHandler(int iSIG);          // Brandon
void toExit();              // Brandon (we cant name it exit)
void path(char *input);       // Colton
void redirect(char *input);   // Colton


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
    //char input[512];  // user input is no more that 512 bytes
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

  //if (signal(SIGINT, signalHandler) == SIG_ERR) // just example
  //  perror("signal");
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
    if (strchr(tmp[i], '|') != NULL)
      piped(tmp[i]);
    // else if command is redirect, redirect(char *input)
    else if (strchr(tmp[i], '<') != NULL || strchr(tmp[i], '>') != NULL)
      redirect(tmp[i]);
    // else if command is cd, cd(char *input)
    else if (strstr(tmp[i], "cd") != NULL)
      cd(tmp[i]);
    // else if command is exit, exit(char *input)
    else if (strstr(tmp[i], "exit") != NULL)
      toExit();
    // else if command is path, path(char *input)
    else if (strstr(tmp[i], "path") != NULL)
      path(tmp[i]);
    // else if command is myhistory, myhistory(char *input)
    else if (strstr(tmp[i], "myhistory") != NULL)
      myhistory(tmp[i]);
    // else if command is alias, alias(char *input)
    else if (strstr(tmp[i], "alias") != NULL)
      alias(tmp[i]);
    // else execute
    else
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
          //exit(0);		// exit so the parent does not execute again
        }
        else if (pid > 0)
          wait((int *)0 );	// wait on child process to end
        else
          perror("fork");
      }

    bzero((char *)cmd, sizeof(*cmd));
    bzero((char *)tmp, sizeof(*tmp));
  }
}

void piped(char *input) // Yafet
{
  int k = 0;  // increment the three commands
  int m = 0;  // increment a, b, c
  int status; // status pipe
  char *tmp[3];   // store array of cmd split by pipe(|)
  pid_t pipe_pid1;
  pid_t pipe_pid2;
  pid_t pipe_pid3;

  // a | b | c
  char *a[4];
  char *b[4];
  char *c[4];

  // split by pipe
  input = strtok(input, "|");
  while(input != NULL)
  {
    tmp[k] = input;
    input = strtok(NULL, "|");
    k++;
  }

  // place split commands in a, b, c
  // A.........
  k = 0;
  tmp[m] = strtok(tmp[m], " ");
  while(tmp[m] != NULL)
  {
    a[k] = tmp[m];
    tmp[m] = strtok(NULL, " ");
    k++;
  }
  a[k] = NULL;

  //B...............B
  k = 0;
  m++;
  tmp[m] = strtok(tmp[m], " ");
  while(tmp[m] != NULL)
  {
    b[k] = tmp[m];
    tmp[m] = strtok(NULL, " ");
    k++;
  }
  b[k] = NULL;

  //C................C
  k = 0;
  m++;
  tmp[m] = strtok(tmp[m], " ");
  while(tmp[m] != NULL)
  {
    c[k] = tmp[m];
    tmp[m] = strtok(NULL, " ");
    k++;
  }
  c[k++] = NULL;

  // use dup2 to pipe
  // fd1[0] reads
  // fd1[1] writes
  int fd1[2], fd2[2];   // for each pipe
  pipe(fd1);      // reads first then write

  pipe(fd2);      // read from fd1 and write stdout

  if ((pipe_pid1 = fork()) == 0)    // child process execute first command
  {
    dup2(fd1[1], fileno(stdout));
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    // close both ends of all created fd(s) (very important)
    execvp(*a, a);  // execute first command
    perror("execvp1\n");
  }
  else if (pipe_pid1 > 0) // parent process forks again
  {
    if ((pipe_pid2 = fork()) == 0)  // second child execute second command
    {
      dup2(fd1[0], fileno(stdin));  // duplicate read end of first -> second pipe
      dup2(fd2[1], fileno(stdout));  // duplicate write end of second -> third pipe
      close(fd1[0]);
      close(fd1[1]);
      close(fd2[0]);
      close(fd2[1]);

      execvp(*b, b);    // execute second command
      perror("execvp2\n");
    }
    else if (pipe_pid2 > 2) // parent process of second
    {
      if ((pipe_pid3 = fork()) == 0)  //third child to execute third command
      {
        dup2(fd2[0], fileno(stdin));  // duplicate read end of last command
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);

        execvp(*c, c);
        perror("execvp3\n");
      }
      else if (pipe_pid3 < 0)
      {
        perror("fork3");
      }
    }
    else
    {
      perror("fork2");
    }

  }
  else
  {
    perror("fork1");  // incase fork does not work
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
  printf("This is myhistory: %s\n", input);
}

void cd(char *input)  // Michael
{
  printf("This is cd: %s\n", input);
}

void alias(char *input) // Michael
{
  printf("This is alias: %s\n", input);
}

void toExit()  // Brandon
{
  printf("This is exit\n");
}

/*void signalHandle(int iSIG)   // Brandon
{
  // handle the signal
}
*/
void path(char *input)  // Colton
{
  printf("This is path: %s\n", input);
}

void redirect(char *input)  // Colton
{
  printf("This is redirect: %s\n", input);
}
