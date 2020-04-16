#include <stdio.h>
#include <unistd.h>	// write() and read() .. man 2 write (or read)
#include <string.h>

#define READ  0	// The index of the read end of the pipe //
#define WRITE 1 // The index of the write end of the pipe //

char* phrase = "Mario goes in the pipe";

int main()
{
	int fd[2], bytesRead;
	char message[100];	// parent process message buffer

	pipe(fd);	// Create unnamed pipe
	if (fork() == 0)	// child process
	{
		close(fd[READ]);	// close read inorder to write
		write(fd[WRITE], phrase, strlen(phrase) + 1);	// plus 1 for NUll
		printf("Child just sent the message to the parent.\n");
		close(fd[WRITE]);
	}
	else 	// parent, reader
	{
		close(fd[WRITE]);
		bytesRead = read(fd[READ], message, 100);
		printf("Parent just read %i bytes: %s\n", bytesRead, message);
		close(fd[READ]);	// close used end
	}

	return 0;
}




// write() if write is open, read will generate SIGPIPE signal to signal broken pipe(EPIPE)
// read()  if read from not pipe open for writing, return 0
// if read() done for empty pipe open for writing by another process, read sleeps until input is available
