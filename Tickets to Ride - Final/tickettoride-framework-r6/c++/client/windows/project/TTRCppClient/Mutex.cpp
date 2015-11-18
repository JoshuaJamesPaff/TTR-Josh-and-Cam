
//_Mutex.cpp____________________________________________________________
#include "Mutex.h"

Mutex::Mutex(void)
{
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex, NULL);
}

void Mutex::acquire(void)
{
	while (true)
	{
		int ret = pthread_mutex_trylock(&mutex);
		if (ret == 0) break;
	}
}

void Mutex::release(void)
{
	pthread_mutex_unlock(&mutex);
}

Mutex::~Mutex(void)
{
	pthread_mutex_destroy(&mutex);
}
