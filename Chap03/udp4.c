/*
* udp4.c
*
* receive UDP datagram using
* recvfrom() function. example
* program #4.
*
* foster <jamescfoster@gmail.com>
*/
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define UDP4_PORT 1234

int main (void)
{
	struct sockaddr_in sin;
	char buf[100];
	int sock = 0;
	int ret = 0;

	sock = socket (AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		printf ("socket() failed.\n");
		return (1);
	}

	memset (&sin, 0x0, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(UDP4_PORT);
	sin.sin_addr.s_addr = INADDR_ANY;

	ret = bind (sock, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf ("bind() failed.\n");
		return (1);
	}

	ret = recvfrom (sock, buf, 100, 0, NULL, NULL);
	if (ret < 0)
	{
		printf ("recvfrom() failed.\n");
		return (1);
	}

;	printf("%s\n", buf);
	close (sock);
	printf("recvfrom() success.\n");

	return (0);
}
