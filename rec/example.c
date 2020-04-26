#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void cd(char *input);
void piped(char *input);
void execute(char *input);

int main()
{
  int k = 0;  // increment the three commands
  int m = 0;  // increment a, b, c
  //char cmd[] = "cat input_file | grep dallas | cut -b 1-10";
  char *ptr;
  char *tmp[3];   // store array of cmd split by pipe(|)

  // a | b | c
  char *a[4];
  char *b[4];
  char *c[4];

  int status;   // status of pipe

  ptr = cmd;
  ptr = strtok(ptr, "|");

  while(ptr != NULL)
  {
    tmp[k] = ptr;
    ptr = strtok(NULL, "|");
    k++;
  }

  //A.............A
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


  // file descriptors for 2 pipes: fd1 for cat-to-grep, fd2 for grep-to-cut
  int fd1[2], fd2[2];

      // make pipe for cat to grep
  pipe(fd1);
  // fd1[0] = read  end of cat->grep pipe (read by grep)
  // fd1[1] = write end of cat->grep pipe (written by cat)

      // make pipe for grep to cut
  // fd2[0] = read  end of grep->cut pipe (read by cut)
  // fd2[1] = write end of grep->cut pipe (written by grep)
  pipe(fd2);
  // fork the first child (to execute cat)
  if (fork() == 0)
  {
    // duplicate write end of cat->grep pipe to stdout
    dup2(fd1[1], fileno(stdout));
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    // close both ends of all created fd# pipes (very important!)


          execvp(*a, a);
  }
    else // parent (assume no error)
      {
          // fork second child (to execute grep)
          if (fork() == 0)
    {
        // duplicate read end of cat->grep pipe to stdin (of grep)
      dup2(fd1[0], fileno(stdin));
u
        // duplicate write end of grep->cut pipe to stdout (of grep)
      dup2(fd2[1], fileno(stdout));
        // close both ends of all created fd# pipes (very important!)
      close(fd1[0]);
      close(fd1[1]);
      close(fd2[0]);
      close(fd2[1]);
        execvp(*b, b);
    }
          else // parent (assume no error)
    {
        // fork third child (to execute cut)
        if (fork() == 0)
              {
                  // duplicate read end of grep->cut pipe to stadin (of cut)
    dup2(fd2[0], fileno(stdin));
                  // close both ends of all created fd# pipes (very important!)
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
                  execvp(*c, c);
              }
    }
      }

    // only the parent gets here, close all pipes and wait for 3 children to finish
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    for (int i = 0; i < 3; i++)
  {
        wait(&status);
  }

}
