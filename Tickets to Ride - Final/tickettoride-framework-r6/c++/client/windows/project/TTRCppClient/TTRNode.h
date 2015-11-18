#ifndef TTRNodeH
#define TTRNodeH

#include "String.h"
#include "TTRPlayer.h"
#include "TTREdge.h"

class TTRPlayer;
class TTREdge;

/**
 * Node class.
 * Represents towns and stations.
 */
class TTRNode{	// station

private:

	/**
	 * Node id.
	 */
	int id;
	
	/**
	 * Node name.
	 */
	String name;
	
	/**
	 * Owning player (if any player built a station here).
	 */
	TTRPlayer *p;
	
	/**
	 * Edge owned by this station (if any player built a station here).
	 */
	TTREdge *e;

public:

	/**
	 * Constructor.
	 * Sets name to parameter value and initializes members.
	 * @param name node name.
	 */
	TTRNode(char *name);

	/**
	 * Setter method for id member.
	 * @param node id.
	 */
	void setId(int id);

	/**
	 * Getter method for id member. 
	 * @return node id.
	 */
	int getId(void);

	/**
	 * Setter method for p member.
	 * @param p player reference.
	 */
	void setPlayer(TTRPlayer *p);

	/**
	 * Getter method for p member.
	 * @return p reference.
	 */
	TTRPlayer *getPlayer(void);

	/**
	 * Setter method for e member.
	 * @param e edge reference.
	 */
	void setEdge(TTREdge *e);

	/**
	 * Getter method for e memeber.
	 * @return e reference.
	 */
	TTREdge *getEdge(void);

	/**
	 * Getter method for name member.
	 * @return node name.
	 */
	String getName(void);

	/**
	  * Destructor.
	  */
	~TTRNode(void);
};
#endif
