// the libraries we will need
#include <stdio.h>
#include <stdlib.h>     // fork()
#include <unistd.h>     // execvp(), getpid()
#include <string.h>     // strlen(), strcmp()...
#include <errno.h>      // perror()
#include <sys/wait.h>   // wait() and waitpid()

int main(int argc, char *argv)
{
  if (argc == 2)
    batch();
    // open file
    // put into a for loop
    // each line become arguement into interactive(char *input)
    // when loop end, exit()

  else if (argc == 1)
  {
    while(1)
    {
      // take input
      // make sure it is not 'quit'
      // pass it to interactive(char *input)
      interactive(char *input);
    }


  }

  else
    printf("usage: ./newshell [file1]\n");
}

void interactive(char *input)
{
  // split input by semicolons
  // execute all commands

  // if command is pipe, piped(char *input)
  // else if command is redirect, redirect(char *input)
  // else if command is cd, cd(char *input)
  // else if command is exit, exit(char *input)
  // else if command is path, path(char *input)
  // else if command is myhistory, myhistory(char *input)
  // else if command is alias, alias(char *input)
  // else execute
}

void piped(char *input)   // by yafet
{
  // fork()
  // make child process execute the pipe
  // make parent wait for child
  // then parent returns

  // becaureful.. it must be parent child
}

void redirect(char *input)
{

}

void cd(char *input)  // by michael 
{

}

void exit(char *input)
{

}

void path(char *input)
{

}

void myhistory(char *input) // by yafet
{

}

void alias(char *input) // by michael
{

}
