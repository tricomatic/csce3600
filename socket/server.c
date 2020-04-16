#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <string.h>
#include <errno.h>
#define sockpath "mysocket"
int main()
{
	int s, len, t, s2, n;
	char message[] ="What is your name ";
	char reply[30];
	char str[20];
	struct sockaddr_un local, remote;
	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
  	{
		perror("socket");
		exit(1);
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, sockpath);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);

	if (bind(s, (struct sockaddr *)&local, len) == -1)
	{
		perror("bind");
		exit(1);
	}

	if (listen(s, 5) == -1)
	{
		perror("listen");
		exit(1);
	}

	printf("waiting for connections....\n");

	t = sizeof(remote);
	if (( s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1)
	{
		perror("accept");
		exit(1);
	}

	printf("connected\n");

	send(s2, message, 20, 0);

	n = recv(s2, str, 20, 0);
	str[n] = '\0';

	printf("Recieved: %s\n", str);

	sprintf(reply, "Hi %s", str);

	send(s2, reply, 20, 0);

	close(s2);

	close(s);
}

// AF_UNIX for process within one machine (alternative is AF_INET)
// SOCK_STREAM for tcp connection and deliverance (alternative is SOCK_DGRAM)
// protocol = 0 except for raw socket
