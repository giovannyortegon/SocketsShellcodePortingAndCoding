/*
 * makeudpsock()
 *
 */
int makeudpsock(char *dst, unsigned short port)
{
	struct sockaddr_in sin;
	struct timeval tv;
	unsigned int taddr = 0;
	int sock = 0;
	int ret = 0;

	taddr = inet_addr(targ);
	if (taddr == INADDR_NONE)
	{
		printf("inet_addr() failed.\n");
		return (-1);
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		printf("socket() failed.\n");
		return (-1);
	}

	memset(&sin, 0x0, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = taddr;

	ret = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("connect() failed.\n");
		return (-1);
	}

	memset(&tv, 0x00, sizeof(tv));

	tv.tv_sec = 10;

	ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	if (ret < 0)
	{
		printf("setsockopt() failed.\n");
		return (-1);
	}

	return (sock);
}
