/*
 * udp2.c
 * send UDP datagram using socket descriptor
 * that has been passed to connect()
 *
 * foster <jamescfoster@gmail.com>
 */

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define UDP2_DST_ADDR "192.168.1.72"
#define UDP2_DST_PORT 1234

int main(void)
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
	sin.sin_port = htons(UDP2_DST_PORT);
	sin.sin_addr.s_addr = inet_addr(UDP2_DST_ADDR);

	ret = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("connect() failed.\n");
		return (1);
	}

	memset(buf, 'A', 100);

	ret = send(sock, buf, 100, 0);
	if (ret != 100)
	{
		printf("send() failed.\n");
		return (1);
	}

	close(sock);
	printf("send() success.\n");

	return (0);
}
