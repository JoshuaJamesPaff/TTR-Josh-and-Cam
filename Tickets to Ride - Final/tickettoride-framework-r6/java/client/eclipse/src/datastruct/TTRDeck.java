
package datastruct;

import java.util.ArrayList;

/**
 * Deck class.
 * Represents a deck of TTRObject (either TTRMission or TTRCard).
 * @param <TTRObject> type of deck.
 */
public class TTRDeck<TTRObject> 
{

	/**
	 * Card list.
	 */
	ArrayList<TTRObject> cards = new ArrayList<TTRObject>(); 
	
	/**
	 * Constructor.
	 */
	public TTRDeck() {}

	/**
	 * Checks if a deck contains cards of color c.
	 * @warning only works on TTRDeck<TTRCard>
	 * @param c color to be searched
	 * @return number of occurences.
	 */
	public int contains(char c)
	{
		int ret = 0;
		for (int i=0; i<cards.size(); i++)
		{
			TTRCard tmp = (TTRCard)cards.get(i);
			if (tmp.getColor() == c) ret++;
		}
		return ret;
	}
	
	/**
	 * Getter method for deck objects.
	 * @param i card position
	 * @return returns TTRObject at position i, null if out of bounds.
	 */
	public TTRObject get(int i)
	{
		if ((i<cards.size()) && (i>=0)) {
			return cards.get(i);
		}
		else {
			return null;
		}
	}
	
	/**
	 * Remove object at position i from deck.
	 * @param i object position.
	 * @return removed object.
	 */
	public TTRObject remove(int i)
	{
		if (( i < cards.size()) &&  (i >= 0)) {
			return cards.remove(i);
		}
		else {
			return null;
		}
	}

	/**
	 * Add object 'o' from deck.
	 * @param o object reference.
	 */
	public void add(TTRObject o)
	{
		cards.add(o);
	}

	/**
	 * Get size of deck.
	 * @return number of cards in deck.
	 */
	public int size() 
	{
		return cards.size();
	}
	
	public ArrayList<TTRObject> getCards() {
		return cards;
	}
}
