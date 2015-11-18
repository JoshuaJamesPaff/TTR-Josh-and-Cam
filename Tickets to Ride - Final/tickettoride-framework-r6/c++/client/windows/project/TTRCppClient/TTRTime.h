
#ifndef TTRTimeH
#define TTRTimeH

#include <windows.h>

/**
 * Time class. Used in random number generator.
 * @see TTRGenerator.
 */
class TTRTime {

public:

	/**
	 * Get time.
	 * @return time passed since epoch
	 */
	static unsigned __int64 get(void);

	/**
	 * Get delta time.
	 * @return time passed since last delta call.
	 */
	static unsigned __int64 delta(void);
};
#endif
