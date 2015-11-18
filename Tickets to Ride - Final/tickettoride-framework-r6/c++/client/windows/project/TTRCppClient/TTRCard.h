#ifndef TTRCardH
#define TTRCardH

#include "TTRColor.c"

/**
 * Card class.
 * Represents a number of game cards of the same color.
 */
class TTRCard {

private:

	/**
	 * Color variable. Takes values from TTRConst.Colors. Initially null.
	 */
	char color;

	/**
	 * Count variable. Represents number of 'color' game cards.
	 */
	int count;

public:

	 /**
	 * Constructor. 
	 * Receives initial card color.
	 * @param color initial color value.
	 */
	TTRCard(char color);

	/**
	 * Constructor.
	 * Receives initial card color and count.
	 * @param color initial color value.
	 * @param count initial count value.
	 */
	TTRCard(char color, int count);

	/**
	 * Getter method for color member.
	 * @return color value
	 */
	char getColor(void);

	/**
	 * Increment method for color member.
	 * @param c value to increase count by.
	 * @return new count member value.
	 */
	int incCount(int c);
	
	/**
	 * Getter method for count member.
	 * @return count member value 
	 */
	int getCount(void);

	void setCount(int n);

	/**
	 * Decrement method for count member.
	 * @param c value to decrement by.
	 * @return new count member value.
	 */
	int decCount(int c);

	/**
	  * Destructor.
	  */
	~TTRCard(void);
};
#endif
