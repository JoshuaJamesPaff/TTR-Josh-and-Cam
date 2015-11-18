
#ifndef TTREdgeH
#define TTREdgeH

#include "String.h"
#include "TTRPlayer.h"
#include "TTRNode.h"

class TTRPlayer;
class TTRNode;

/**
 * Route class.
 * Represents a route between two 
 */
class TTREdge{

private:

	/**
	 * Edge id.
	 */
	int id;

	/**
	 * Route owners. 
	 * Double routes may be owned by two players.  
	 */
	TTRPlayer *(owner)[2];

	/**
	 * Players that own a station on this route.
	 */
	TTRPlayer *(station)[2];

	/**
	 * Edge source node reference.
	 */
	TTRNode *nodeA;
	
	/**
	 * Edge destination node.
	 */
	TTRNode *nodeB;
	
	/**
	 * Route point value.
	 */
	int val;
	
	/**
	 * Route colors. Double routes have two colors.
	 */
	char col[2];

	/**
	 * Number of engines required to claim this route.
	 */
	int engs;
	
	/**
	 * Number of cars required to claim this route.
	 */
	int cars;

	/**
	 * Tunnel indicator. 1 if tunnel, 0 otherwise.
	 */
	int tunnel;

public:

	/**
	 * Radix value. Used to generate edge id from node ids.
	 */
	static const int radix = 100;
		
	/**
	 * 2 if double route, 1 otherwise. 
	 */
	int nColors;

	/**
	 * Constructor.
	 * Initializes class members with params.
	 * @param a source node reference.
	 * @param b destination node reference.
	 * @param val route point value. 
	 * @param col0 route color 0.
	 * @param col1 route color 1.
	 * @param engs number of engines required to claim this route.
	 * @param cars number of cars required to claim this route.
	 * @param tunnel 1 if this route is a tunnel, 0 otherwise.
	 */
	TTREdge(TTRNode *a, TTRNode *b, 
		  int val, char col0, 
		  char col1, int loco,
		  int  cars, int tunnel);

	/**
	 * Sets id based on source and destination node ids.
	 * @param i source node id.
	 * @param j destination node id.
	 */
	void setId(int i, int j);

	/**
	 * Getter method for id member.
	 * @return id member value.
	 */
	int getId(void);

	/**
	 * Getter method for nodeA member.
	 * @return nodeA member reference.
	 */
	TTRNode *getNodeA(void);

	/**
	 * Getter method for nodeB member.
	 * @return nodeB member reference.
	 */
	TTRNode *getNodeB(void);

	/**
	 * Getter method for value member.
	 * @return edge point value.
	 */
	int getValue(void);

	/**
	 * Getter method for engines member.
	 * @return number of engines required to claim this route.
	 */
	int getEngines(void);

	/**
	 * Getter method for cars member.
	 * @return number of cars required to claim this route.
	 */
	int getCars(void);

	/**
	 * Getter method for tunnel member.
	 * @return 1 if tunnel, 0 otherwise.
	 */
	int getTunnel(void);

	/**
	 * Getter method for route color.
	 * @param i specifies which route is being inquired (0 for single routes).
	 * @return route color, '0' if out of bounds.
	 */
	char getColor(int i);

	/**
	 * Set edge owner (when edge is claimed).
	 * @param p owning player.
	 * @param c requested color
	 */
	void setOwner(TTRPlayer *p, char c);

	/**
	 * Get edge owner (player that claimed this edge).
	 * @param p which owner to query.
	 * @return player that owns this edge.
	 */
	TTRPlayer *getOwner(int p);

	/**
	 * Method that builds a station on this edge for player p.
	 * @param p new station owner.
	 */
	void buildStation(TTRPlayer *p);

	/**
	 * Getter method for station owners.
	 * @param i which owner to query.
	 * @return owning player method.
	 */
	TTRPlayer *getStation(int i);

	/**
	  * Destructor.
	  */
	~TTREdge(void);
};
#endif
