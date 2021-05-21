#include<stdio.h>
#include<WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#define STRING_MAX  1024
#define MAX         64000

char *client_send(char *targetip, int port)
{
    WSADATA wsaData;
    WORD wVersionRequested;
    struct hostent      *target_ptr;
    struct sockaddr_in  sock;
    SOCKET  MySock;

    wVersionRequested = MAKEWORD(2, 2);

    if (WSAStartup(wVersionRequested, &wsaData) < 0)
    {
        printf("############# Error! ################\n");
        printf("Your ws2_32.dll is outdated.\n");
        printf("Download and install the most recent\n");
        printf("version of ws_32.dll.\n");

        WSACleanup();
        exit(1);
    }

    MySock = socket(AF_INET, SOCK_STREAM, 0);

    if (MySock == INVALID_SOCKET)
    {
        printf("Socket error!\r\n");
        closesocket(MySock);
        WSACleanup();
        exit(1);
    }

    target_ptr = gethostbyname(targetip);
    if (target_ptr == NULL)
    {
        printf("Resolve of %s failed, please try again.\n", targetip);
        closesocket(MySock);
        WSACleanup();
        exit(1);
    }
    memcpy(&sock.sin_addr.s_addr, target_ptr->h_addr, target_ptr->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    
    if (connect(MySock, (struct sockaddr *)&sock, sizeof(sock)))
    {
        printf("Couldn't connect to host.\n");
        closesocket(MySock);
        WSACleanup();
        exit(1);
    }

    char *recvString = (char *) malloc(sizeof(char) * MAX);
    int nret;
    nret = recv(MySock, recvString, MAX + 1, 0);
    char *output = (char *)malloc(sizeof(char) * MAX);
    strcpy(output, "");

    if (nret == SOCKET_ERROR)
    {
        printf("Attempt to receive data FAILED.\n");
    }
    else
    {
        strncat(output, recvString, nret);
        free(recvString);
    }

    closesocket(MySock);
    WSACleanup();
    return (output);
    free(output);
}

int main(int argc, char *argv[])
{
    int port = 80;
    char *targetip;
    char *output = NULL;

    if (argc < 2)
    {
        printf("ClientApp usage: \r\n");
        printf("    %s <TargetIp> [port]\r\n", argv[0]);
        return (0);
    }
    targetip = argv[1];
    if (argc >= 3)
    {
        port = atoi(argv[2]);
    }
    printf("%s", client_send(targetip, port));
    return (0);
}