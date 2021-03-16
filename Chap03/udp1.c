/*
 * udp1.c
 *
 * create UDP socket example program #1
 *
 * foster <jamescfoster@gmail.com>
 */
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main(void)
{
	int sock = 0;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		printf("socket() faild.\n");
	}
	else
	{
		close(sock);
		printf("socket() success.\n");
	}
	return (0);
}
