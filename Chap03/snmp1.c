/*
* snmp1.c
*
* snmp scanner example program #1.
*
* foster <jamescfoster@gmail.com>
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define		SNMP1_DEF_PORT	161
#define		SNMP1_DEF_CMN	"public"

#define		SNMP1_BUF_SIZE	0X0400
/*
 * hexdisp()
 */
void hexdisp (char *buf, int len)
{
	char tmp[16];
	int x = 0;
	int y = 0;

	printf("\n");

	for (x = 0; x < len; x++)
	{
		tmp[x % 16] = buf[x];

		if ((x + 1) % 16 == 0)
		{
			for (y = 0; y < 16; ++y)
			{
				printf("%02X ", tmp[y] & 0xFF);
			}

			for (y = 0; y < 16; ++y)
			{
				printf("%c", isprint(tmp[y]) ? tmp[y] : '.');
			}

			printf("\n");
		}
	}

	if ((x % 16) != 0)
	{
		for (y = 0; y < (x % 16); ++y)
		{
			printf("%02X ", tmp[y] & 0xFF);
		}

		for ( y = (x % 16); y < 16; ++y)
		{
			printf("    ");
		}

		for (y = 0; y < (x % 16); ++y)
		{
			printf("%c", isprint(tmp[y]) ? tmp[y] : '.');
		}
	}

	printf("\n");
}

/*
 * makeget()
 *
 */

#define SNMP1_PDU_HEAD "\x30\x00\x02\x01\x00\x04"
#define SNMP1_PDU_TAIL "\xa0\x1c\x02\x04\x7e\x16\xa2\x5e" \
						"\x02\x01\x00\x02\x01\x00\x30\x0e" \
						"\x30\x0c\x06\x08\x2b\x06\x01\x02" \
						"\x01\x01\x05\x00\x05\x00"

int makegetreq (char *buf, int blen, int *olen, char *comn)
{
	int hlen = sizeof(SNMP1_PDU_HEAD) - 1;
	int tlen = sizeof(SNMP1_PDU_TAIL) - 1;
	int clen = strlen(comn);
	int len = 0;

	len = hlen + 1 + clen + tlen;
	if (len > blen)
	{
		printf("insufficient buffer space (%d, %d).\n",
				blen, len);
		return (-1);
	}

	memset(buf, 0x00, blen);
	memcpy(buf, SNMP1_PDU_HEAD, hlen);
	memcpy(buf + hlen + 1, comn, clen);
	memcpy(buf + hlen + 1 + clen, SNMP1_PDU_TAIL, tlen);

	buf[0x01] = 0x23 + clen;
	buf[hlen] = (char) clen;

	*olen = len;

	return (0);
}
/*
 * dores()
 */

int dores (int sock)
{
	char buf[SNMP1_BUF_SIZE];
	int ret = 0;

	ret = recvfrom(sock, buf, SNMP1_BUF_SIZE, 0, NULL, NULL);
	if (ret < 0)
	{
		printf("recv() failed.\n");
		return (-1);
	}

	hexdisp(buf, ret);

	return (0);
}
/*
 * doreq ()
 */
int doreq (int sock, char *comn)
{
	char buf[SNMP1_BUF_SIZE];
	int len = 0;
	int ret = 0;

	ret = makegetreq(buf, SNMP1_BUF_SIZE, &len, comn);
	if (ret < 0)
	{
		printf("makegetreq() failed.\n");
		return (-1);
	}

	ret = send(sock, buf, len, 0);
	if (ret != len)
	{
		printf("send() failed.\n");
		return (-1);
	}

	return (0);
}
/*
 * makeudpsock()
 */
int makeudpsock(char * targ, unsigned short port)
{
	struct  sockaddr_in sin;
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

	return (sock);

}
/*
 * scan()
 */
int scan (char *targ, unsigned short port, char *cname)
{
	int sock = 0;
	int ret = 0;

	sock = makeudpsock(targ, port);
	if (sock < 0)
	{
		printf("makeudpsocket() failed.\n");
		return (-1);
	}

	ret = doreq(sock, cname);
	if (ret < 0)
	{
		printf("doreq() failed.\n");
		return (-1);
	}

	ret = dores(sock);
	if (ret < 0)
	{
		printf("dores() failed.\n");
		return (-1);
	}

	return (0);
}

/*
 * usage()
 */
void usage(char *prog)
{
	printf("snmp1 00.00.01\r\n");
	printf("usage: %s -t target_ip < -p target_port >" \
		   "< -c community_name >\n", prog);
	printf("example: %s -t 127.0.0.1 -p 161 -c public\n\n", prog);
}

int main(int argc, char *argv[])
{
	unsigned short port = SNMP1_DEF_PORT;
	char *targ = NULL;
	char *comn = SNMP1_DEF_CMN;
	char ch = 0;
	int ret = 0;

	opterr = 0;

	while ((ch = getopt(argc, argv, "t:p:c")) != -1)
	{
		switch(ch)
		{
			case 't':
				targ = optarg;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'c':
				comn = optarg;
				break;
			case '?':
			default:
				usage(argv[0]);
				return (-1);
		}
	}

	if (targ == NULL)
	{
		usage(argv[0]);
		return (-1);
	}

	printf("using: target: %s; port: %d"\
			"community name: \"%s\" \n", targ, port, comn);

	ret = scan(targ, port, comn);
	if (ret < 0)
	{
		printf("scan() failed. \n");
		return (-1);
	}

	printf("scan complete.\n");

	return (0);
}
