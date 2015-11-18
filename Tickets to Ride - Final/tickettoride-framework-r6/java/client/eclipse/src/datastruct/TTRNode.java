
package datastruct;

/**
 * Node class.
 * Represents towns and stations.
 */
public class TTRNode {
	
	/**
	 * Node id.
	 */
	private int  id = 0;
	
	/**
	 * Node name.
	 */
	private String name;
	
	/**
	 * Owning player (if any player built a station here).
	 */
	private	TTRPlayer p;
	
	/**
	 * Edge owned by this station (if any player built a station here).
	 */
	private	TTREdge e;
	
	/**
	 * Constructor.
	 * Sets name to parameter value and initializes members.
	 * @param name node name.
	 */
	public TTRNode(String name)
	{
		this.name = new String(name);
		p = null; e = null;
		id = -1;
	}
	
	/**
	 * Setter method for id member.
	 * @param node id.
	 */
	public void setId(int id)	
	{
		this.id = id;
	}
	
	/**
	 * Getter method for id member. 
	 * @return node id.
	 */
	public int getId()
	{
		return id;
	}
	
	/**
	 * Setter method for p member.
	 * @param p player reference.
	 */
	void setPlayer(TTRPlayer p) 
	{
		this.p = p;
	}
	
	/**
	 * Getter method for p member.
	 * @return p reference.
	 */
	TTRPlayer getPlayer()		
	{
		return p;
	}

	/**
	 * Setter method for e member.
	 * @param e edge reference.
	 */
	void setEdge(TTREdge e)		
	{
		this.e = e;
	}
	
	/**
	 * Getter method for e memeber.
	 * @return e reference.
	 */
	TTREdge getEdge()
	{
		return e;
	}

	/**
	 * Getter method for name member.
	 * @return node name.
	 */
	public String getName()
	{
		return name;
	}
}
