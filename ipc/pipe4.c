#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int fd;
	// open file for redirection
	fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
	dup2(fd, 1);	// duplicate fd to standard output
	close(fd);	// close to save space

	execvp(argv[2], &argv[2]);	// invoke program; will inherit stdout
	perror("main");		// if it does not execute
}
