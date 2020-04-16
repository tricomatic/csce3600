/* 	This program demonstrates dup and dup2 system calls.
	"test.txt" must be present.
*/

#include <stdio.h>
#include <sys/types.h>	// open()
#include <fcntl.h>	// open()
#include <sys/file.h>
#include <unistd.h>

int main()
{
	int fd1, fd2, fd3;
	fd1 = open("test.txt", O_RDWR | O_TRUNC);
	printf("fd1 = %i\n", fd1);
	write(fd1, "what's", 6);

	fd2 = dup(fd1);		// makes a copy of fd1
	printf("fd2 = %i\n", fd2);
	write(fd2, " up", 3);

	close(0);	// close stdin
	fd3 = dup(fd1);	// another copy of fd1. also picks up stdin

	printf("fd3 = %i\n", fd3);
	write(0, " doc", 4);	// 0 belongs to fd3 now so it write from fd3, not screen

	dup2(fd3, fileno(stderr));
	write(2, "?", 1);
	write(0, "?\n", 2);
}
