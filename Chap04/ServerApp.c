#include<stdio.h>
#include<WinSock2.h>
#include<stdlib.h>
#include<string.h>

#pragma comment (lib, "ws_2_32.lib")

#define STRING_MAX  2048
#define MAX         640000
#define MAX_CON     16

int server(int port, char * send_string)
{
    WSADATA wsaData;
    WORD wVersionRequested;
    SOCKET MyServer;
    int nret;

    wVersionRequested = MAKEWORD(2, 2);

    if (WSAStartup(wVersionRequested, &wsaData) < 0)
    {
        printf("################### ERROR ###################\n");
        printf("Your ws2_32.dll is tool old to use this application. \n");
        printf("Go to microsofts web site to download the most recent.\n");
        printf("version of ws2_32.dll.                         \n");

        WSACleanup();
        return (0);
    }

    MyServer = socket(AF_INET, SOCK_STREAM, 0);
    if (MyServer == INVALID_SOCKET)
    {
        nret = WSAGetLastError();
        printf("Socket did not connect.\n");
        closesocket(MyServer);
        WSACleanup();
        return (0);
    }
    struct sockaddr_in serverInfo;
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(port);

    nret = bind(MyServer, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
    if (nret == SOCKET_ERROR)
    {
        nret = WSAGetLastError();
        printf("ERROR ON BIND\n");

        closesocket(MyServer);
        WSACleanup();
        return (0);
    }

    nret = listen(MyServer, MAX_CON);
    if (nret == SOCKET_ERROR)
    {
        nret = WSAGetLastError();
        printf("Error on listen\n");
        closesocket(MyServer);
        WSACleanup();
        return (0);
    }

    SOCKET MyClient;
    MyClient = accept(MyServer, NULL, NULL);

    if (MyClient == INVALID_SOCKET)
    {
        nret = WSAGetLastError();
        printf("Error at accept().");
        closesocket(MyServer);
        closesocket(MyClient);
        WSACleanup();

        return (0);
    }

    char *sendStr = (char *) malloc(sizeof(char) * STRING_MAX);
    strcpy(sendStr, "");
    strcpy(sendStr, send_string);

    nret = send(MyClient, sendStr, strlen(sendStr) -1, 0);
    if (nret == SOCKET_ERROR)
        printf("Message could not be sent.\n");
    else
        printf("Message sent.\n");

    free(sendStr);
    closesocket(MyClient);
    closesocket(MyServer);

    WSACleanup();
    return (1);
}
int main(int argc, char * argv[])
{
    int port = 777;
    char * targetip;
    char * output = NULL;

    if (argc < 2)
    {
        printf("ServerApp usage: \r\n");
        printf("    %s [port]\r\n", argv[0]);
        return (0);
    }

    targetip = argv[1];
    if (argc >= 2)
    {
        port = atoi(argv[1]);
    }
    int up = 1;
    char sendStr[STRING_MAX];

    strcpy(sendStr, "\r\n Hello World! \r\n\r\n");
    printf("Starting Server...\n");

    do
    {
        up = server(port, sendStr);
    } while (up);

    return (0);
}