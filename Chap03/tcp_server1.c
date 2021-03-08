/*
 * tcp_server1.c
 *
 * Create TCP server socket, accept
 * one TCP client connection using
 * socket(), bind(), listen() and
 * accept().
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/type.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	struct sockaddr_in sin;
	struct sockaddr_in csin;
	socklen_t len = sizeof(struct sockaddr);
	short port = 0;
	int csock = 0;
	int sock = 0;
	int ret = 0;

	if (argc != 2)
	{
		printf("usage: %s: port\n");
		return (1);
	}

	memset(&sin, 0x0, sizeof(struct sockaddr_in *));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sock, (struct sockaddr *)&sin, sizeof(sin));

	if (ret < 0)
	{
		printf("TCP server listen() failed.\n");
		close(sock);
		return (1);
	}

	printf("TCP server listening.\n");

	memset(&csin, 0x0, (struct sockaddr));
}
