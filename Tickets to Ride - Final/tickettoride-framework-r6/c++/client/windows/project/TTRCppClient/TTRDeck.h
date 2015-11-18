
#ifndef TTRDeckH
#define TTRDeckH

#include "ArrayList.h"
#include "TTRTime.h"
#include "TTRTime.h"
#include "TTRCard.h"

template <class Object>

/**
 * Deck class.
 * Represents a deck of TTRObject (either TTRMission or TTRCard).
 * @param <TTRObject> type of deck.
 */
class TTRDeck {

private:

	/**
	 * Card list.
	 */
	ArrayList<Object> *cards;

public:

	/**
	 * Constructor.
	 */
	TTRDeck(void)
	{
		cards = new ArrayList<Object>();
	}

	/**
	 * Checks if a deck contains cards of color c.
	 * @warning only works on TTRDeck<TTRCard>
	 * @param c color to be searched
	 * @return number of occurences.
	 */
	int contains(char c)
	{
		int ret = 0;
		for (int i=0; i<cards->size(); i++)
		{
			TTRCard *tmp = cards->get(i);
			if (tmp->getColor() == c) ret++;
		}
		return ret;
	}

	/**
	 * Getter method for deck objects.
	 * @param i card position
	 * @return returns TTRObject at position i, null if out of bounds.
	 */
	Object *get(int i)
	{
		return cards->get(i);
	}

	/**
	 * Remove object at position i from deck.
	 * @param i object position.
	 * @return removed object.
	 */
	Object *remove(int i)
	{
		return cards->remove(i);
	}

	/**
	 * Add object 'o' from deck.
	 * @param o object reference.
	 */
	void add(Object *o)
	{
		cards->add(o);
	}

	/**
	 * Get size of deck.
	 * @return number of cards in deck.
	 */
	int size(void) 
	{
		return cards->size();
	}

	/**
	  * Destructor.
	  */
	~TTRDeck(void)
	{
		if (cards) delete cards;
	}
};

#endif
