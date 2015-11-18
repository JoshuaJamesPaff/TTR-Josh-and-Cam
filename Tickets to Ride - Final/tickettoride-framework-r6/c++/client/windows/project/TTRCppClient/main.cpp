
#include <conio.h>
#include "RandomSolution.h"

int main(int argc, char **argv)
{
	if (Socket::initSocketLib()!=0)
	{
		printf("Error initialising socket library\n");
	}

	RandomSolution *s = new RandomSolution(
		"CppUser", "CppUser", "CppAgent", "127.0.0.1", 1337);
	Thread::sleep(100); s->start(); Thread::sleep(100); s->go();
	
	printf("press any key ...\n");
	_getch(); 
	s->stop();
	delete s; 

	Socket::cleanupSocketLib();

	return 0;
}
