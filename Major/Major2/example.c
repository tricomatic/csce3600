#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
  char command[512];
  int i = 0;
  char *cmd[15];
  printf("prompt> ");
  scanf("%[^\n]%*c", command);

  char *ptr = strtok(command, ";");
  while(ptr != NULL)
  {
    cmd[i] = ptr;
    ptr = strtok(NULL, ";");
    i++;
  }
  cmd[i] = NULL;

  execvp(cmd[0], cmd);

  for (int j = 0; j < i; j++)
  {
    printf("%s\n", cmd[j]);
  }
  printf("%d\n", i);
  return 0;
}
