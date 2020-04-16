#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <errno.h>

#define sock_path "mysocket"

int main()
{
	int s, len, n;
	char str[20];
	char msg[20];
	char final[20];
	struct sockaddr_un remote;

	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	printf("Trying to connect...\n");

	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, sock_path);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);

	if (connect(s, (struct sockaddr *)&remote, len) == -1)
	{
		perror("connect");
		exit(1);
	}

	printf("connected..\n");

	n = recv(s, str, 20, 0);
	str[n] = '\0';

	printf("%s", str);

	scanf("%s", msg);

	send(s, msg, 20, 0);

	n = recv(s, final, 20, 0);
	final[n] = '\0';
	printf("%s\n", final);

	close(s);

	return 0;
}
