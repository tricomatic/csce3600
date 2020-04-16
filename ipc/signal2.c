#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void hello(int iSig)
{
	printf("Hello World!\n");
}

int main()
{
	signal(SIGINT, hello);

	while(1);	// infinite loop
	return 0;
}
