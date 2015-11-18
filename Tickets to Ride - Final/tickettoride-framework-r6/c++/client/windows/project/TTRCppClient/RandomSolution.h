
#ifndef RandomSolutionH
#define RandomSolutionH

#include "TTRGenerator.h"
#include "TTRClient.h"

/**
 * Class that attempts to play the game by using randomly selected actions.
 */
class RandomSolution : public TTRClient
{
protected:

	/**
	 * Symbolic constant to draw from open deck. 
	 */
	static const int DRAW_FROM_OPEN_DECK= 0;
	
	/**
	 * Symbolic constant to draw from deck. 
	 */
	static const int DRAW_FROM_DECK		= 1;
	
	/**
	 * Symbolic constant to draw missions. 
	 */
	static const int DRAW_MISSIONS		= 2;
	
	/**
	 * Symbolic constant to claim a route. 
	 */
	static const int CLAIM_ROUTE		= 3;
	
	/**
	 * Symbolic constant to build a station. 
	 */
	static const int BUILD_STATION		= 4;

	/**
	 * Random number generator.
	 */
	TTRGenerator *myRand;

	/**
	 * Action performed flag.
	 */
	bool actionPerformed;
	int drawCounter;
	char sixColor;

public:

	/**
	 * Constructor.
	 * @see TTRClient.TTRClient()
	 * @param user user name.
	 * @param pass password.
	 * @param agent agent name.
	 * @param name thread name.
	 * @param host server address.
	 * @param port server port.
	 */
	RandomSolution(char *user, char *pass, 
		char *agent, char *host, int port);

	/**
	 * Main method. This is triggered when it is our turn.
	 */
	/**
	<pre>
	// Set action performed flag to false as we have not acted this turn.
	actionPerformed = false;
		
	// If (firstMissions==true) reject last received mission.
	if (firstMissions) discardLastMission();
	else 	
		// As long as we have not performed an action, attempt to choose an action to perform.
		while ((!actionPerformed)&&(play)&&(myTurn))
		{
			 // Randomly select an action.
			int action = myRand->get()%5;
			
			// Let's see what we chose:
			switch (action) 
			{
				// Attempt to draw a card from the open deck.
				case DRAW_FROM_OPEN_DECK: attemptToDrawFromDeck(); break;
				
				// Attempt to draw a card from the blind deck.
				case DRAW_FROM_DECK: attemptToDrawFromDeck(); break;
				
				// Attempt to draw missions.
				case DRAW_MISSIONS: attemptToDrawMissions(); break;
				
				// Attempt to claim a route.
				case CLAIM_ROUTE: attemptToClaimRoute(); break;
				
				// Attempt to build a station.
				case BUILD_STATION: attemptToBuildStation(); break;
				
				// We have no idea what we tried to do; better try again. 
				default: break;
			}
		}
	</pre>
	  */

	bool countColors();
	void myMethod();
	void discardMissions();

	/**
	 * Discards last received mission.
	 */
	/**
	<pre>
	int ret = 0;
	ArrayList<TTRMission> *m = myPlayer->getMissionArray();
	while ((ret != 1)&&(play)){
		ret = rejectMissions(m->get(m->size()-1), (TTRMission *)0);
		actionPerformed = true;
	}
	</pre>
	  */
	void discardLastMission(void);

	/**
	 * Attempt to draw from deck.
	 */
	/**
	<pre>
	// Draw the card.
	int ret = drawCardFromDeck();
	if ((ret==1)||(ret==0))
	{
		// If successful, try to draw another card from the blind deck.
		int ret2 = 0;
		// Keep trying until successful (as long as we are still playing).
		while ((ret2 != 1)&&(play))
			ret2 = drawCardFromDeck();
		// Set actionPerformed flag to true.
		actionPerformed = true;
	}
	</pre>
	  */
	void attemptToDrawFromDeck(void);
	
	/**
	 * Attempt to draw missions. Keeps them all.
	 */
	/**
	<pre>
	// Attempt to draw missions.
	int ret = drawMissions();
	if (ret>0)
	{
		// If successful
		ret = 0;
		// While not successful attempt to keep all missions.
		while ((ret != 0)&&(play))
			ret = keepAllMissions();
	}
	// Set actionPerformed flag to true.
	actionPerformed = true;
	</pre>
	  */
	void attemptToDrawMissions(void);
	
	/**
	 * Attempt to claim a route.
	 */
	/**
	<pre>
	// Initialize edge to claim.
	TTREdge *edge = (TTREdge *)0; 
	// Get number of nodes on map.
	int n = myMap->getNAdj();
	// Initialize source and destination node ids.
	int s=0, d=0;
	// As long as the edge we chose is null, randomly pick a different edge.
	while (edge == (TTREdge *)0)
	{
		s = myRand->get() % n;
		d = myRand->get() % n;
		edge = myMap->getAdj()[s][d];
	}
	// Get edge color.
	char color = edge->getColor(myRand->get()%2);
	// Get out player's cards in hand.
	TTRCard **myCards = myPlayer->getCards();
	// Get number of cars and engines required to claim route.
	int cars = edge->getCars(), engs = edge->getEngines();
	// Search for cards that match the edge's color.
	for (int i=0; i<nColors-1; i++)
	{
		if ((myCards[i]->getColor() == color) || (color == '*'))
		{
			TTRCard *card = myCards[i];
			if (card->getCount() < cars)
			{
				// Figure out how to pay for edge.
				engs += cars - card->getCount();
				cars = card->getCount();
			}
			color = card->getColor();
			break;
		}
	}
	// Attempt to claim edge.
	int ret = claimRoute(s, d, color, cars, engs);
	// If successful, set actionPerformed flag to true
	if (ret>=0) {
		// If more cards are required
		if (ret>0)
			// While not successful attempt to pass.
			while ((ret != 0)&&(play)) 
				ret = claimPass();
		actionPerformed = true;
	}
	</pre>
	  */
	void attemptToClaimRoute(void);
	
	/**
	 * Attempt to build a station.
	 */
	/**
	<pre>
	// If we have no stations left, move on
	if (myPlayer->getNStations() == 0) return;
	// Initialize edge to build station on.
	TTREdge *edge = (TTREdge *)0; 
	// Get number of nodes on map.
	int n = myMap->getNAdj();
	// Initialize source and destination nodes.
	int s=0, d=0;
	// Randomly pick an edge.
	while (edge == (TTREdge *)0)
	{
		s = myRand->get()%n; 
		d = myRand->get()%n;
		edge = myMap->getAdj()[s][d];
	}
	// Get number of required cards to build station.
	int req = 4 - myPlayer->getNStations();
	char color = '0'; 
	// Get first card in card array to have enough cards to build station.
	TTRCard **myCards = myPlayer->getCards();
	for (int i=0; i<nColors-1; i++)
	{
		if (myCards[i]->getCount() >= req)
		{
			color = myCards[i]->getColor();
			break;
		}
	}
	// Attempt to build station.
	int ret = buildStation(s, d, color, req, 0);
	// If successful, set actionPerformed flag to true.
	if (ret==1) actionPerformed = true;
	</pre>
	  */
	void attemptToBuildStation(void);

	/**
	  * Destructor.
	  */
	virtual ~RandomSolution(void);
};

#endif



