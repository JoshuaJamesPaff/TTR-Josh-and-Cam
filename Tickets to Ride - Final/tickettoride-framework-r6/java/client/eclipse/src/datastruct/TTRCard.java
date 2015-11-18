
package datastruct;

/**
 * Card class.
 * Represents a number of game cards of the same color.
 */
public class TTRCard 
{

	/**
	 * Color variable. Takes values from TTRConst.Colors. Initially null.
	 */
	private char color = '0';
	
	/**
	 * Count variable. Represents number of 'color' game cards.
	 */
	private int  count =  0;
	
	/**
	 * Constructor. 
	 */
	public TTRCard(){}
	
	/**
	 * Constructor. 
	 * Receives initial card color.
	 * @param color initial color value.
	 */
	public TTRCard(char color)
	{
		this.color = color;
	}
	
	/**
	 * Constructor.
	 * Receives initial card color and count.
	 * @param color initial color value.
	 * @param count initial count value.
	 */
	public TTRCard(char color, int count)
	{
		this.color = color; this.count = count;
	}
	
	/**
	 * Getter method for color member.
	 * @return color value
	 */
	public char getColor()
	{
		return color;
	}
	
	/**
	 * Increment method for color member.
	 * @param c value to increase count by.
	 * @return new count member value.
	 */
	public int incCount(int c)
	{
		return count+=c;
	}
	
	/**
	 * Getter method for count member.
	 * @return count member value 
	 */
	public int getCount() 
	{
		return count;
	}
	
	/**
	 * Decrement method for count member.
	 * @param c value to decrement by.
	 * @return new count member value.
	 */
	public int decCount(int c) 
	{
		return count-=c;
	}
}
