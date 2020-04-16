#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	int i;
	system("whoami");
	char *cmd[] = {"who", "ls", "date"};
	printf("0=who 1=ls 2=date\n");
	scanf("%d", &i);

	execlp(cmd[i], cmd[i], NULL);
	printf("execlp failed\n");
	return 0;

}
