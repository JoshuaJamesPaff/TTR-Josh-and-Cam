#ifndef TTRGeneratorH
#define TTRGeneratorH

#include "TTRTime.h"

/**
 * Random number generator class.
 */
class TTRGenerator {

private:

	static long double a, m;

	static long double D, S;

	/**
	 * Reset value. After this many random numbers, the generator is reseeded.
	 */
	static int rst;

	/**
	 * Number of times this generator has been used.
	 */
	int count;

	/**
	 * Last seed.
	 */
	long z;

public:

	/**
	 * Constructor. Calls reset.
	 */
	TTRGenerator(void);

	/**
	 * Generates integer random number.
	 */
	long get(void);

	/**
	 * Generates floating point random number in [0,1]
	 */
	double frn(void);

	/**
	 * Seeds the generator.
	 */
	void reset(void);

	/**
	 * Destructor.
	 */
	~TTRGenerator(void);
};
#endif
