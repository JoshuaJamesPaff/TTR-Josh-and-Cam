
package datastruct;

/**
 * Route class.
 * Represents a route between two 
 */
public class TTREdge {

	/**
	 * Edge id.
	 */
	int id;
	
	/**
	 * Route owners. 
	 * Double routes may be owned by two players.  
	 */
	TTRPlayer[] owner 	= new TTRPlayer [2];
	
	/**
	 * Players that own a station on this route.
	 */
	TTRPlayer[] station = new TTRPlayer [2];
	
	/**
	 * Edge source node reference.
	 */
	TTRNode nodeA;
	
	/**
	 * Edge destination node.
	 */
	TTRNode nodeB;
	
	/**
	 * Route colors. Double routes have two colors.
	 */
	char [] col = new char [2];
	
	
	/**
	 * Route point value.
	 */
	int val; 
	
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
	
	/**
	 * Radix value. Used to generate edge id from node ids.
	 */
	public static final int radix = 100;
	
	/**
	 * 2 if double route, 1 otherwise. 
	 */
	public int nColors;
	
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
	public TTREdge(TTRNode a, TTRNode b, int val, 
	char col0, char col1, int engs, int cars, int tunnel)
	{
		this.nodeA = a;  this.nodeB = b;
		this.val = val; 
		this.col[0] = col0;  this.col[1] = col1;
		if (col1 == '0') nColors = 1; 
		else nColors = 2;
		this.engs = engs; this.cars = cars;
		this.tunnel = tunnel;
		owner[0] = owner[1] = null;
		station[0] = station[1] = null;
	}
	
	/**
	 * Sets id based on source and destination node ids.
	 * @param i source node id.
	 * @param j destination node id.
	 */
	public void setId(int i, int j)
	{
		id = i*radix + j;
	}
	
	/**
	 * Getter method for id member.
	 * @return id member value.
	 */
	public int getId() 
	{
		return id;
	}
	
	/**
	 * Getter method for nodeA member.
	 * @return nodeA member reference.
	 */
	public TTRNode getNodeA() 
	{
		return nodeA;
	}
	
	/**
	 * Getter method for nodeB member.
	 * @return nodeB member reference.
	 */
	public TTRNode getNodeB() 
	{
		return nodeB;
	}
	
	/**
	 * Getter method for value member.
	 * @return edge point value.
	 */
	public int getValue() 
	{
		return val;
	}
	
	/**
	 * Getter method for engines member.
	 * @return number of engines required to claim this route.
	 */
	public int getEngines() 
	{
		return engs;
	}
	
	/**
	 * Getter method for cars member.
	 * @return number of cars required to claim this route.
	 */
	public int getCars() 
	{
		return cars;
	}
	
	/**
	 * Getter method for tunnel member.
	 * @return 1 if tunnel, 0 otherwise.
	 */
	public int getTunnel() 
	{
		return tunnel;
	}
	
	/**
	 * Set edge owner (when edge is claimed).
	 * @param p owning player.
	 */
	public void setOwner(TTRPlayer p, char c) 
	{
		/*
		if ((c == col[0]) && (owner[0] == null)) {
			owner[0] = p;
		}
		else if ((c == col[1]) && (owner[1] == null)) {
			owner[1] = p;
		}
		else if (('*' == col[0]) && ('*' == col[1])) {
			if (owner[0] == null) {
				owner[0] = p;
			}
			else if (owner[1] == null) {
				owner[1] = p;
			}
		}
		*/
		if (((c==col[0]) || (col[0]=='*')) && (owner[0]==null))
			owner[0] = p;
		else if ((nColors > 1)&&((c==col[1])||(col[1]=='*')) && (owner[1]==null))
			owner[1] = p;
	}
	
	/**
	 * Get edge owner (player that claimed this edge).
	 * @param p which owner to query.
	 * @return player that owns this edge.
	 */
	public TTRPlayer getOwner(int p)
	{
		if ((p<2)&&(p>=0)) 
			return owner[p];
		else return null;
	}
	
	/**
	 * Method that builds a station on this edge for player p.
	 * @param p new station owner.
	 */
	public void buildStation(TTRPlayer p)
	{
		if (station[0]==null) station[0] = p;
		else station[1] = p;
	}
	
	/**
	 * Getter method for station owners.
	 * @param i which owner to query.
	 * @return owning player method.
	 */
	public TTRPlayer getStation(int i)
	{
		if ((i>=0)&&(i<2)) return station[i];
		else return null;
	}
	
	/**
	 * Getter method for route color.
	 * @param i specifies which route is being inquired (0 for single routes).
	 * @return route color, '0' if out of bounds.
	 */
	public char getColor(int i) 
	{
		if ((i>=0)&&(i<2)) return col[i];
		else return '0';
	}
}
