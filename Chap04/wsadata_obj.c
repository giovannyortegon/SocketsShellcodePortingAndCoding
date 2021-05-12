/* WSADATA */
WSADATA wsaData;
WORD wVersionRequested;
wVersionRequested = MAKEWORD(2, 0);

WSAStartup(wVersionRequested, &wsaData);
if (WSAStartup(wVersionRequested), &wsaData < 0)
{
	printf("Wrong version");
	exit(1);
}

SOCKET MySocket;
MySocket = socket(AF_INET, SOCK_STREAM, 0);
MySocket = socket(AF_INET, SOCK_DGRAM, 0);

struct  hostent * target_ptr;

target_ptr = gethostbyname(targetip);
if (target_ptr = gethostbyname(targetip) == NULL)
{
	printf("Can not resolve name.");
	exit(1);
}

struct sockaddr_in sock;

memcpy(&sock.sin_addr.s_addr, target_ptr->h_addr, target_ptr->h_length);
sock.sin_family = AF_INET;
sock.sin_port = htons(port);

connect (MySocket, (struct sockaddr *)&sock, sizeof(sock));
if (connect(MySocket, (struct sockaddr *)&sock, sizeof(sock)))
{
	printf("Failed to connect.");
	exit(1);
}

char *recv_string = new char[MAX];
int nret = 0;
nret = recv(MySocket, recv_string, MAX -1, 0);
if (nret = recv(MySocket, recv_string, MAX -1, 0) <= 0)
{
	printf("Did not recover any data.");
	exit(1);
}

char send_string[] = "\n\r Hello World \n\r\n\r";
int nret = 0;
nret = send(MySocket, send_string, sizeof(send_string) -1, 0);
if ( nret = send(MySocket, send_string, sizeof(send_string) -1, 0) <= 0)
{
	printf("Could not send any data.");
	exit(1);
}

socketaddr_in serverInfo;
serverInfo.sin_family = AF_INET;
serverInfo.sin_addr.s_addr = INADDR_ANY;
listen(MySocket, 10);
SOCKET	NewSock;

NewSock = accept(MySocket, NULL, NULL);

closesocket(MySocket);
WSACleanup();
