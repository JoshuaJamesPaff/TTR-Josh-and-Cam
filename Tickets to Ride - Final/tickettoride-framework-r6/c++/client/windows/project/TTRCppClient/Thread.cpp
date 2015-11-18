
#include "Thread.h"

Thread::Thread(void){}

void Thread::start(void)
{
	if (int errcode = pthread_create(&thread, 0, &Thread::init, this))
	{
		// pthread_create error.
	}
}

void Thread::sleep(int millis){
	Sleep((DWORD)millis);
}

void *Thread::init(void *t)
{
	((Thread *)t)->run();
	return (void *)0;
}

#include <stdio.h>
void Thread::stop(void){
	int *status;
	if (int errcode = pthread_join(thread, (void **)&status))
	{
		// pthread_join error.
		printf("join error [%i]\n", errcode);
	}
}

Thread::~Thread(void){}