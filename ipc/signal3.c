#include <stdio.h>
#include <signal.h>
#include <sys/signal.h>

int main()
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		printf("Cannot ignore SIGKILL...\n");
	}

	// infinte loop
	while(1);
	return 0;
}
