
#ifndef TTRMapH
#define TTRMapH

#include "tinyxml.h"

#include "TTRMission.h"
#include "String.h"
#include "TTRPlayer.h"
#include "TTRTime.h"
#include "TTRNode.h"
#include "TTREdge.h"
#include "TTRDeck.h"

class TTRMission;
class TTRPlayer;
class TTREdge;

/**
 * Map class.
 * Encodes all available information about a game map.
 */
class TTRMap {

private:

	/**
	 * Number of nodes / cities on map.
	 */
	int nAdj;
	
	/**
	 * Adjacency matrix. TTREdge object if route exists, null otherwise.
	 */
	TTREdge ***adj; 
	
	/**
	 * Node / City list.
	 */
	ArrayList<TTRNode> *nodeList;
	
	String name;

	/**
	 * Open deck.
	 */
	TTRDeck<TTRCard>	*openDeck;

public:

	/**
	 * Number of players in game.
	 */
	int nPlayers;

	/**
	 * Constructor.
	 */
	TTRMap(void);
	
	/**
	 * Load from file method.
	 * Loads nodes, edges and missions from an XML file.
	 * @param filename source file name.
	 * @return error code.
	 */
	int loadFromFile(char *filename);

	/**
	 * Getter method for adjacency matrix.
	 * @return adjacency matrix reference.
	 */
	TTREdge ***getAdj(void); 
	
	/**
	 * Getter method for adjacency matrix size.
	 * @return adjacency matrix size.
	 */
	int getNAdj(void);
	
	/**
	 * Node list getter method.
	 * @return node list reference.
	 */
	ArrayList<TTRNode> *getNodeList(void);
	
	/**
	 * Name getter method.
	 * @return map name.
	 */
	String getName(void);
	
	/**
	 * Get node by id.
	 * @param n node id.
	 * @return node reference.
	 */
	TTRNode *getNode(int n);

	/**
	 * Open deck getter method.
	 * @return open deck reference.
	 */
	TTRDeck<TTRCard> *getOpenDeck(void);

	/**
	 * Claim route dummy.
	 * @param p
	 * @param i
	 * @param j
	 * @param c
	 * @param cars
	 * @param engs
	 */
	void claimRouteDummy(TTRPlayer *p, int s, int d, char col, int cars, int engs);
	
	/**
	 * Executes a player's claim.
	 * @param p attempting layer.
	 * @param i source node.
	 * @param j destination node.
	 * @param c card color.
	 * @param cars number of color cards used.
	 * @param engs number of engines used.
	 * @return error code.
	 */
	bool Claim(TTRPlayer *Player, int Nod1, int Nod2, char Color, int Cars, int Engines);

	bool BuildStation(TTRPlayer *Player, int StationNode, int ConnectionNode, char Color, int Cars, int Engines);

	/**
	 * Removes a card from the open deck.
	 * @param c card color.
	 */	
	void removeCardFromOpenDeck(char c);
	
	/**
	 * Refreshes the open deck based on string parameter.
	 * @param s encodes new open deck contents.
	 */
	void refreshOpenDeck(String  s);

	/**
	 * Build station dummy.
	 * @param ttrPlayer
	 * @param parseInt
	 * @param parseInt2
	 * @param charAt
	 * @param parseInt3
	 * @param parseInt4
	 * 
	 * @warning deprecated.
	 * @warning unimplemented.
	 */
	void buildStationDummy(TTRPlayer *p, int s, int d, char c, int cars, int engs);

	/**
	 * Number of cards in deck;
	 */
	int nCardsInDeck(void);
	
	/**
	 * Cleans up memory.
	 */
	void cleanup(void);

	/**
	 * Destructor.
	 */
	~TTRMap(void);
};

#endif
