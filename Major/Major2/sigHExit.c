#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void sigctch(int sig)// tell user what signal was caught.
{
	if(sig == SIGINT)
	{
		printf("\ncaught SIGINT at base\n");
	}
	else if(sig == SIGTSTP)
	{
		printf("\ncaught SIGTSTP at base\n");
	}
	else if(sig == SIGQUIT)
	{
		printf("\ncaught SIGQUIT at base\n");
	}
}

void sigHPr()
{
	signal(SIGINT, sigctch);
	signal(SIGTSTP, sigctch);
	signal(SIGQUIT, sigctch);
	//signal(SIGINT, sigctch);
}

void sigHCh()
{
	//pid_t pid = getpid();
	setpgrp();
	//tcsetpgrp(0, getpid());

	//printf("\n@@@@@@@process = %d\n", getpid());
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	//signal(SIG, SIG_DFL);
}

void toExit()
{
	exit(0);
}
