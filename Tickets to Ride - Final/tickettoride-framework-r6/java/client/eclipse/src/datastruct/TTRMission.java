
package datastruct;

/**
 * Mission class.
 * Encodes information relevant to mission cards.
 */
public class TTRMission {
	
	/**
	 * Source node reference
	 */
	private TTRNode nodeA;
	
	/**
	 * Destination node reference.
	 */
	private TTRNode nodeB;
	
	/**
	 * Mission point value.
	 */
	private int 	value;

		/**
		 * Constructor.
		 * Sets members ot param values.
		 * @param a mission source node. 
		 * @param b mission destination node. 
		 * @param val mission point value.
		 */
	public TTRMission(TTRNode a, TTRNode b, int val)
	{
		nodeA = a; nodeB = b; value = val;
	}

	/**
	 * Getter method for nodeA member.
	 * @return nodeA reference.
	 */
	public TTRNode getNodeA()
	{
		return nodeA;
	}

	/**
	 * Getter method for nodeB member.
	 * @return nodeB reference.
	 */
	public TTRNode getNodeB()
	{
		return nodeB;
	}

	/**
	 * Getter method for value member. 
	 * @return mission point value.
	 */
	public int getValue() 	
	{
		return value;
	} 
}
