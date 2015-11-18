
#ifndef SocketH
#define SocketH

#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#include "String.h"

/**
 * Socket wrapper class. Uses socket API.
 */
class Socket
{
private:

	/**
	 * Buffer size.
	 */
	static int const buffLen = 1;

	static WORD wVersionRequested;
	
	static WSADATA wsaData;

	/**
	 * Socket address struct.
	 */
	struct sockaddr_in address;
	
	/**
	 * Host struct.
	 */
	struct hostent * host;
	
	/**
	 * Read buffer.
	 */
	char buffer[buffLen];

	/**
	 * Host name.
	 */
	String hostName;
	
	/**
	 * Socket struct.
	 */
	SOCKET sock;
	
	/**
	 * Port.
	 */
	int port;

protected:

public:

	/**
	 * Initialize socket library.
	 */
	static int initSocketLib(void);

	/**
	 * Cleanup socket library.
	 */
	static int cleanupSocketLib(void);

	/**
	 * Constructor.
	 * @param hostName host name or address.
	 * @param port server port.
	 */
	Socket(char *hostName, int port);

	/**
	 * Attempt to connect to server.
	 * @return true if successful, false otherwise.
	 */
	bool connectToServer(void);

	/**
	 * Receive message from server.
	 */
	String recvFromServer(void);

	/**
	 * Send message to server.
	 */
	void sendToServer(String s);

	/**
	 * Destructor.
	 */
	virtual ~Socket(void);
};

#endif