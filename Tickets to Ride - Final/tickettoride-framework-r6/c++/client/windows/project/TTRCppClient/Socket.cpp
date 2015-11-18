
#include "Socket.h"

WORD Socket::wVersionRequested;

WSADATA Socket::wsaData;

/**
 * Initialize windows socket library.
 * @return 0 on success, error code otherwise.
 */
int Socket::initSocketLib(void)
{
	wVersionRequested = MAKEWORD( 1, 1 );
	int ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0) return ret;
	float socklib_ver;
	socklib_ver = HIBYTE( wsaData.wVersion ) / 10.f;
	socklib_ver += LOBYTE( wsaData.wVersion );
	if (socklib_ver < 1.1f) 
	{
		printf("Socket library version earlier than 1.1\n");
		cleanupSocketLib(); exit(0);
	}
	return 0;
}

int Socket::cleanupSocketLib(void)
{
	return WSACleanup();
}

/**
 * Constructor.
 * Initializes socket.
 */
Socket::Socket(char *hostName, int port)
{
	this->hostName = hostName;
	this->port = port;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) 
	{
		printf("invalid socket\n");
		// handle error
	}
	address.sin_family = AF_INET;		// internet
	address.sin_port = htons(port);

	if ((host = gethostbyname(hostName)) == NULL)
	{
		printf("unable to get host by name\n");
		// handle error
	} else {
		address.sin_addr.s_addr=*((unsigned long *) host->h_addr);
	}
	char optval = 1; int optlen = sizeof(optval);
	int err = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
	if (err < 0)
	{
		printf("unable to set keepalive\n");
		// handle error
	}
}

/**
 * Connect to server.
 * @return true if successful, false otherwise.
 */
bool Socket::connectToServer(void)
{
	/*
	sockaddr addr;
	addr.sa_family = AF_INET;
	sprintf(addr.sa_data, "%i.%i.%i.%i", 
		address.sin_addr.S_un.S_un_b.s_b1,
		address.sin_addr.S_un.S_un_b.s_b2,
		address.sin_addr.S_un.S_un_b.s_b3,
		address.sin_addr.S_un.S_un_b.s_b4);
	*/
	if ((connect(sock,(struct sockaddr *) &address, 
		sizeof(address))) != 0) return false;
	else return true;
}

String Socket::recvFromServer(void)
{
	String ret = "";
	buffer[0] = 0;
	while (buffer[0] != '\n'){
		int err = recv(sock, buffer, 1, 0);
		if (err <= 0){
			printf("receive error\n"); break;
		} else {
			ret += buffer[0];
		}
	}
	return ret;
	/*
	String tmp = "";
	memset(buffer, 0, buffLen);
	int err = recv(sock, buffer, buffLen, 0);

	if (err <= 0)
	{
		// handle error
		printf("receive error\n");
		tmp = ""; 
		memset(buffer, 0, buffLen);
		return tmp;

	} else {
		tmp += buffer;
		memset(buffer, 0, buffLen);
		return tmp;
	}
	*/
}

void Socket::sendToServer(String s)
{
	String tmp = s + "\n";
	int err = send(sock, tmp.data, tmp.length(), 0);
	if (err <= 0)
	{
		printf("send error\n");
		// handle error
	} 
	//else printf("%i:%i characters sent\n", err, tmp.Length());
}

/**
 * Destructor.
 */
Socket::~Socket(void){ }