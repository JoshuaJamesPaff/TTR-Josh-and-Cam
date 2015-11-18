
#ifndef ThreadH
#define ThreadH

#include <windows.h>
#include <pthread.h>

/**
 * Thread wrapper class. Uses pthread library.
 */
class Thread
{
private:

	/**
	 * Initialize thread. Runs Thread::start();
	 */
	static void *init(void *t);

protected:

	/**
	 * Thread info.
	 */
	pthread_t thread;

public:

	Thread(void);

	/**
	 * Start thread.
	 */
	void start(void);

	/**
	 * Thread behavior method.
	 */
	virtual void run(void)=0;

	static void sleep(int m);

	/**
	 * Stop thread.
	 */
	void stop(void);

	/**
	 * Destructor.
	 */
	virtual ~Thread(void);
};

#endif