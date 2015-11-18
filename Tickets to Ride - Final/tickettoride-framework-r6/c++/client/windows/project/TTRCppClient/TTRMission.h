
#ifndef TTRMissionH
#define TTRMissionH

#include "String.h"
#include "TTRNode.h"

class TTRNode;

/**
 * Mission class.
 * Encodes information relevant to mission cards.
 */
class TTRMission {

private:

	/**
	 * Source node reference
	 */
	TTRNode *nodeA;
	
	/**
	 * Destination node reference.
	 */
	TTRNode *nodeB;
	
	/**
	 * Mission point value.
	 */
	int value;

public:

	/**
	 * Constructor.
	 * Sets members ot param values.
	 * @param a mission source node. 
	 * @param b mission destination node. 
	 * @param val mission point value.
	 */
	TTRMission(TTRNode *a, TTRNode *b, int val);

	/**
	 * Getter method for nodeA member.
	 * @return nodeA reference.
	 */
	TTRNode *getNodeA(void);

	/**
	 * Getter method for nodeB member.
	 * @return nodeB reference.
	 */
	TTRNode *getNodeB(void);

	/**
	 * Getter method for value member. 
	 * @return mission point value.
	 */
	int getValue(void);
	
	/**
	  * Destructor.
	  */
	~TTRMission(void);
};
#endif
