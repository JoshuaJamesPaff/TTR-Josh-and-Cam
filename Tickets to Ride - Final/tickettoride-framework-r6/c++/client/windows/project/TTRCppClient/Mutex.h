
//_Mutex.h______________________________________________________________
#ifndef MutexH
#define MutexH

#include <pthread.h>

/**
 * Pthread mutex wrapper class.
 */
class Mutex
{

protected:

	/**
	 * Pthread mutex.
	 */
	pthread_mutex_t		mutex;
	
	/**
	 * Pthread mtex attributes.
	 */
	pthread_mutexattr_t	attr;

public:
	
	/**
	 * Void constructor. Initialises mutex.
	 */
	Mutex(void);

	/**
	 * Acquire mutex method (lock).
	 */
	void acquire(void);

	/**
	 * Release mutex method (unlock).
	 */
	void release(void);

	/**
	 * Destructor.
	 */
	~Mutex(void);
};

#endif