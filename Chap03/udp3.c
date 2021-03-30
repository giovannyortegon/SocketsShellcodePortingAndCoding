/*
* udp3.c
*
* send UDP datagram using socket
* descriptor and sendto(). example
* program #3.
*
* foster <jamescfoster@gmail.com>
*/
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define UDP3_DST_ADDR "127.0.0.1"
#define UDP3_DST_PORT 1234

int main()
{
	struct sockaddr_in sin;
	char buf[100];
	int sock = 0;
	int ret = 0;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		printf("socket() failed.\n");
		return (1);
	}

	memset(&sin, 0x0, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(UDP3_DST_PORT);
	sin.sin_addr.s_addr = inet_addr(UDP3_DST_ADDR);

	memset(buf, 'A', 100);

	ret = sendto(sock, buf, 100, 0, (struct sockaddr *)&sin, sizeof(sin));
	if (ret != 100)
	{
		printf("sendto() failed.\n");
		return (1);
	}

	close(sock);
	printf("sendto() success.\n");

	return (0);
}
