#include <stdio.h>
#include <assert.h>	// abort the program if assertion is false
#include <signal.h>
#include <unistd.h>

void myHandler(int iSig)
{
	printf("in myHandler with argument #d\n", iSig);
}

int main()
{
	void (*pfRet)(int) = signal(SIGINT, myHandler);	// signal(type of sig, fxn)
	assert(pfRet != SIG_ERR);

	printf("Entering an infinite loop\n");
	while (1)
	{
		printf(".");
		usleep(1000);
	}
	return 0;
}

// goes in infinite loop until killed by -9
