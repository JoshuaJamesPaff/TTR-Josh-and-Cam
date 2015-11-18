
#ifndef TTRClientThreadH
#define TTRClientThreadH

#include "String.h"
#include "Socket.h"
#include "Thread.h"

/**
 * Client application class
 * Establishes a connection, reads strings from socket and allows message sending.
 */
class TTRClientThread : public Thread
{
private:

	/**
	 * Server socket.
	 */
	Socket *socket;
	
protected:

	/**
	 * Client quit flag.
	 */
	bool quit;

	/**
	 * Server host name.
	 */
	String  hostName;
	
	/**
	 * Client connected flag.
	 */
	bool connected;
	
	/**
	 * Server port.
	 */
	int port;

	/**
	 * Write string to socket.
	 * @param s string to be written.
	 */
	void send(String  s);

	/**
	 * Read string from socket. 
	 * @return read string.
	 */
	String  receive(void);
	
	/**
	 * Attempt to connect to server.
	 * @return true if success, false otherwise.
	 */
	bool connect(void);

public:

	/**
	 * Constructor.
	 * @warning does not initialize server variables.
	 */
	TTRClientThread(void);

	/**
	 * Constructor.
	 * @param name thread name.
	 * @param host server name/address.
	 * @param port server port.
	 */
	TTRClientThread(char *name, 
			char *host, int port);

	/**
	 * Set server.
	 * @param host host name / address.
	 */
	void setHost(char *host);

	/**
	 * Set port.
	 * @param port port.
	 */
	void setPort(int port);

	/**
	 * Thread run method. Started when start() is called.
	 * Connects to server and reads strings from  socket.
	 * If disconnected calls onDisconnect(); otherwise
	 * calls commandHandler(read string).
	 */
	void run(void);

	/**
	 * Abstract method. Handles commands.
	 * @param cmd command to handle.
	 */
	virtual void commandHandler(String  cmd)=0;

	/**
	 * Abstract method. Handles disconnects.
	 */
	virtual void onDisconnect(void)=0;

	/**
	 * @warning unused.
	 */
	virtual void onStatus(void)=0;

	/**
	 * Destructor.
	 */
	virtual ~TTRClientThread(void);
};

#endif
