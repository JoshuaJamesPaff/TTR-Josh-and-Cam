
#include "TTRClientThread.h"

TTRClientThread::TTRClientThread()
{
	hostName = "127.0.0.1";
	connected = false; 
	this->port = 1337; 
	quit = false;
	socket = (Socket *)0;
}

TTRClientThread::TTRClientThread(char *name, char *host, int port) : Thread()
{
	hostName = host;
	connected = false;
	this->port = port; 
	quit = false;
	socket = (Socket *)0;
}

void TTRClientThread::setHost(char *host) 
{
	hostName = host;
}

void TTRClientThread::setPort(int port)
{
	this->port = port;
}

bool TTRClientThread::connect(void) {
	socket = new Socket(hostName.data, port);
	if (socket->connectToServer())
		return connected = true;
	else {
		delete socket;
		socket = (Socket *)0;
		return connected = false;
	}
}

void TTRClientThread::run() {
	while (!quit){
		if (connected)
		{
			String buffer = (String)"";

			buffer = receive();
			if (buffer == "") 
			{
				connected = false;
				onDisconnect();
			}
			else commandHandler(buffer);
		} else {
			if (!connect()) Thread::sleep(100);
		}
	}
}

void TTRClientThread::send(String  s)
{
	printf("sending %s\n", s.data);
	socket->sendToServer(s);
}

String TTRClientThread::receive(void){
	String recv = socket->recvFromServer();
	printf("received %s\n", recv.data);
	return recv.trim();
}

TTRClientThread::~TTRClientThread(void){
	if (socket) delete socket;
}