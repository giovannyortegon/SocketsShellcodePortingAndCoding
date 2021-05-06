/*
* rpc1.c
*
* TCP RPC program scanner example program #1.
*
*
* foster <jamescfoster@gmail.com>
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <string.h>

#define RPC1_BUF_SIZE 0x0400
#define RPC1_DEF_CTO_SEC 0x0005
#define RPC1_DEF_RTO_SEC 0x0005
#define RPC1_ID_HEAD    "\x80\x00\x00\x28\x00\x00\x00\x12"\
                        "\x00\x00\x00\x00\x00\x00\x00\x02"
#define RPC1_ID_TAIL    "\x00\x00\x00\x00\x00\x00\x00\x00" \
                        "\x00\x00\x00\x00\x00\x00\x00\x00" \
                        "\x00\x00\x00\x00\x00\x00\x00\x00"

/*
* program numbers
*/
unsigned int progid[] =
{0x000186A0, 0x000186A1, 0x000186A2, 0x000186A3,
0x000186A4, 0x000186A5, 0x000186A6, 0x000186A7,
0x000186A8, 0x000186A9, 0x000186AA, 0x000186AB,
0x000186AC, 0x000186AD, 0x000186AE, 0x000186AF,
0x000186B1, 0x000186B2, 0x000186B3, 0x000186B4,
0x000186B5, 0x000186B6, 0x000186B7, 0x000186B8,
0x000186B9, 0x000186BA, 0x000186BB, 0x000186BC,
0x000186BD, 0x000186C5, 0x000186C6, 0x000186E4,
0x000186F3, 0x0001877D, 0x00018788, 0x0001878A,
0x0001878B, 0x00018799, 0x000249F1, 0x000493F3,
0x00049636, 0x30000000, 0x00000000
};

/*
* hexdisp()
*
*
*/
void hexdisp (char *buf, int len)
{
	char tmp[16];
	int x = 0;
	int y = 0;

	for(x=0; x < len; ++x)
	{
		tmp[x % 16] = buf[x];

		if((x + 1) % 16 == 0)
		{
			for(y=0; y < 16; ++y)
			{
				printf("%02X ", tmp[y] & 0xFF);
			}

			for(y=0; y < 16; ++y)
			{
				printf("%c", isprint(tmp[y])? tmp[y] : '.');
			}

		printf("\n");
		}
	}

	if((x % 16) != 0)
	{
		for(y=0; y < (x % 16); ++y)
		{
			printf("%02X ", tmp[y] & 0xFF);
		}

		for(y=(x % 16); y < 16; ++y)
		{
			printf(" ");
        }
		for(y=0; y < (x % 16); ++y)
		{
			printf("%c", isprint(tmp[y]) ? tmp[y] : '.');
		}
	}
}
/*
* rpcidport()
*
*
*/
int rpcidport (int sock, unsigned int *id, int verb)
{
    unsigned int cur = 0;
    char buf[RPC1_BUF_SIZE];
    int hlen = sizeof(RPC1_ID_HEAD) - 1;
    int tlen = sizeof(RPC1_ID_TAIL) - 1;
    int clen = sizeof(unsigned int);
    int len = hlen + clen + tlen;
    int ret = 0;
    int x = 0;

    for(x=0; progid[x] != 0x00000000; ++x)
    {
        cur = htonl(progid[x]);
        memset(buf, 0x00, RPC1_BUF_SIZE);
        memcpy(buf, RPC1_ID_HEAD, hlen);
        memcpy(buf + hlen, &cur, clen);
        memcpy(buf + hlen + clen, RPC1_ID_TAIL, tlen);

        ret = send(sock, buf, len, 0);
        if(ret != len)
        {
            if(verb)
            {
                printf("send() failed.\n");
            }
            return(-1);
        }
        ret = recv(sock, buf, RPC1_BUF_SIZE, 0);
        if(ret >= 28)
        {
            if(buf[0x04] == 0x00 &&
               buf[0x05] == 0x00 &&
               buf[0x06] == 0x00 &&
               buf[0x07] == 0x12 &&
               buf[0x0B] == 0x01 &&
               buf[0x1B] != 0x01)
            {
                *id = progid[x];
                return(0);
            }
        }
        else
        {
            // unexpected response, probably not RPC
            // service, return from function...
            return(0);
        }
    }
        return(0);
}

/*
* makesock()
*
*
*/
int makesock(unsigned int taddr, unsigned short port,
        unsigned int cto_sec, long rto_sec, int verb)
{
    struct sockaddr_in sin;
    struct timeval tv;
    int sock = 0;
    int ret = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        if(verb)
        {
            printf("socket() failed.\n");
        }
        return(-1);
    }
    memset(&sin, 0x00, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = taddr;
    alarm(cto_sec);

    ret = connect(sock, (struct sockaddr *) &sin, sizeof(sin));
    alarm(0);
    if(ret < 0)
    {
        close (sock);

        if(verb)
        {
            printf("connect () %d.%d.%d.%d:%d failed.\n",
                    (taddr >> 0x00) & 0xFF, (taddr >> 0x08) & 0xFF,
                    (taddr >> 0x10) & 0xFF, (taddr >> 0x18) & 0xFF,
                    port);
        }
        return(-1);
    }
    memset(&tv, 0x00, sizeof(tv));
    tv.tv_sec = rto_sec;

    ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if(ret < 0)
    {
        close(sock);
        if(verb)
        {
            printf("setsockopt() failed.\n");
        }
        return(-1);
    }
    return(sock);
}

/*
* rpcid()
*
*
*/
int rpcid (unsigned int taddr, unsigned short port,
           unsigned int cto_sec, long rto_sec, int verb)
{
    unsigned int id = 0;
    int sock = 0;
    int ret = 0;

    sock = makesock(taddr, port, cto_sec, rto_sec, verb);

    if(sock < 0)
    {
        if(verb)
        {
            printf("makesock () failed.\n");
        }
        return(0);
    }

    ret = rpcidport(sock, &id, verb);

    if(ret < 0)
    {
        close(sock);

        if(verb)
        {
            printf("rpcidport() failed @ %d.%d.%d.%d:%d\n",
                    (taddr >> 0x00) & 0xFF, (taddr >> 0x08) & 0xFF,
                    (taddr >> 0x10) & 0xFF, (taddr >> 0x18) & 0xFF, port);
        }
        return (0);
    }
    close(sock);

    if(id != 0)
    {
        printf("RPC %d [%08X] @ %d.%d.%d.%d:%d\n", id, id,
                (taddr >> 0x00) & 0xFF, (taddr >> 0x08) & 0xFF,
                (taddr >> 0x10) & 0xFF, (taddr >> 0x18) & 0xFF,
                port);
    }
    return(0);
}
/*
* scan()
*
*
*/
int scan (char *targ, unsigned short lport, unsigned short hport,
          unsigned int cto_sec, long rto_sec, int verb)
{
    unsigned int taddr = 0;
    int ret = 0;
    taddr = inet_addr(targ);

    if(taddr == INADDR_NONE)
    {
        if(verb)
        {
            printf("inet_addr() failed.\n");
        }
        return(-1);

    }

    while(lport <= hport)
    {
        ret = rpcid(taddr, lport, cto_sec, rto_sec, verb);

        if(ret < 0)
        {
            if(verb)
            {
                printf("rpcid() failed.\n");
            }
            return(-1);
        }
        ++lport;
    }
    return(0);
}
/*
* parse()
*
*
*/
int parse (char *sprt, unsigned short *lport, unsigned short *hport)
{
    char *tmp = NULL;

    tmp = (char *) strchr(sprt, '-');

    if(tmp == NULL)
    {
        *hport = *lport = (unsigned short) atoi(sprt);
    }
    else
    {
        *tmp = '\0';
        *lport = (unsigned short) atoi(sprt);
        ++tmp;
        *hport = (unsigned short) atoi(tmp );
    }

    if(*lport == 0 || *hport == 0 || (*lport > *hport))
    {
        return(-1);
    }
    return(0);
}

/*
* sighandler()
*
*
*/
void sighandler (int sig)
{
}

/*
* usage()
*
*
*/
void usage(char *prog)
{
    printf("rpc1 00.00.01\n");
    printf("usage : %s -t target_ip -p port_range\n", prog);
    printf("example: %s -t 127.0.0.1 -p 1-1024\n\n" , prog);
}
int main(int argc, char *argv[])
{
    unsigned short lport = 0;
    unsigned short hport = 0;
    unsigned int cto_sec = RPC1_DEF_CTO_SEC;
    char *targ = NULL;
    char *sprt = NULL;
    char *tmp = NULL;
    char ch = 0;
    long rto_sec = RPC1_DEF_RTO_SEC;
    int verb = 0;
    int ret = 0;

    signal(SIGALRM, sighandler);
    signal(SIGPIPE, sighandler);

    opterr = 0;
    while((ch = getopt(argc, argv, "t:p:c:r:v")) != -1)
    {
        switch(ch)
        {
            case 't':
                targ = optarg;
                break;
            case 'p':
                sprt = optarg;
                break;
            case 'c':
                cto_sec = (unsigned int) atoi(optarg);
                break;
            case 'r':
                rto_sec = (long) atoi(optarg);
                break;
            case 'v':
                verb = 1;
                break;
            case '?':
            default:
                usage(argv[0]);
                return(1);

        }
    }
    if(targ == NULL || sprt == NULL)
    {
        usage(argv[0]);
        return(1);
    }
    ret = parse(sprt, &lport, &hport);

    if(ret < 0)
    {
        printf("parse() failed.\n");
        return(1);
    }
    printf("\nusing: target: %s; lport: %d; hport: %d\n\n",
            targ, lport, hport);
    ret = scan(targ, lport, hport, cto_sec, rto_sec, verb);

    if(ret < 0)
    {
        printf("scan() failed.\n");
        return(1);
    }

    printf("scan complete.\n");
    return(0);
}
