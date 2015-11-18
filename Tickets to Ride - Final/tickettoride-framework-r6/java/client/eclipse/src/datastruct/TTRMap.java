
package datastruct;

import java.io.File;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

/**
 * Map class.
 * Encodes all available information about a game map.
 */
public class TTRMap {

	/**
	 * Map name.
	 */
	private String 		 name;
	
	/**
	 * Number of nodes / cities on map.
	 */
	private int nAdj;
	
	/**
	 * Adjacency matrix. TTREdge object if route exists, null otherwise.
	 */
	private TTREdge [][] adj; 
	
	/**
	 * Node / City list.
	 */
	private ArrayList<TTRNode> 	nodeList = new ArrayList<TTRNode>();

	/**
	 * Open deck.
	 */
	private TTRDeck<TTRCard> openDeck = new TTRDeck<TTRCard>();
	
	/**
	 * Number of players in game.
	 */
	public int nPlayers = 0;
	
	/**
	 * Constructor.
	 */
	public TTRMap(){}
	
	/**
	 * Load from file method.
	 * Loads nodes, edges and missions from an XML file.
	 * @param filename source file name.
	 * @return error code.
	 */
	public int loadFromFile(String filename)
	{
		try {
			File file = new File(filename);
			
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(file);
			
			Element root = doc.getDocumentElement();
			NodeList nodes = root.getElementsByTagName("node");
			for (int i=0; i<nodes.getLength(); i++)
			{
				Element node = (Element) nodes.item(i);
				TTRNode ttrNode = new TTRNode(node.getAttribute("name"));
				nodeList.add(ttrNode);
				ttrNode.setId(nodeList.size()-1);
			}
			
			nAdj = nodeList.size();
			adj = new TTREdge [nAdj][nAdj];
			for (int i=0; i<nAdj; i++) for (int j=0; j<nAdj; j++) adj[i][j] = null;
			
			NodeList edges = root.getElementsByTagName("edge");
			for (int k=0; k<edges.getLength(); k++)
			{
				Element edge = (Element) edges.item(k);
				String 	node1 = edge.getAttribute("node1"),
						node2 = edge.getAttribute("node2");
				
				int i=-1, j=-1;
				for (int l=0; l<nodeList.size(); l++)
					if (nodeList.get(l).getName().equals(node1))
					{
						i = l; if (j!=-1) break;
					} 
					else if (nodeList.get(l).getName().equals(node2))
					{
						j = l; if (i!=-1) break;
					} else ;
				
				if ((i!=-1) && (j!=-1))
				{
					char col1 = edge.getAttribute("color1").charAt(0), 
						 col2 = edge.getAttribute("color2").charAt(0);
					
					adj[i][j] = new TTREdge(nodeList.get(i), nodeList.get(j), 
						Integer.parseInt(edge.getAttribute("value")), col1, col2,
						Integer.parseInt(edge.getAttribute("eng")), 
						Integer.parseInt(edge.getAttribute("cars")), 
						Integer.parseInt(edge.getAttribute("tunnel")));
					adj[i][j].setId(i, j);
					adj[j][i] = adj[i][j];
				} else {
					// error message
				}
			}
		} catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 1;
	}

	/**
	 * Getter method for adjacency matrix.
	 * @return adjacency matrix reference.
	 */
	public TTREdge [][] getAdj()
	{
		return adj;
	}
	
	/**
	 * Getter method for adjacency matrix size.
	 * @return adjacency matrix size.
	 */
	public int getNAdj()
	{
		return nAdj;
	}
	
	/**
	 * Node list getter method.
	 * @return node list reference.
	 */
	public ArrayList<TTRNode> getNodeList()
	{
		return nodeList;
	}
	
	/**
	 * Name getter method.
	 * @return map name.
	 */
	public String getName()
	{
		return name;
	}

	/**
	 * Open deck getter method.
	 * @return open deck reference.
	 */
	public TTRDeck<TTRCard> getOpenDeck()
	{
		return openDeck;
	}

	/**
	 * Claim route dummy.
	 * @param p
	 * @param i
	 * @param j
	 * @param c
	 * @param cars
	 * @param engs
	 */
	public void claimRouteDummy(TTRPlayer p, int i, int j, char c, int cars, int engs)
	{

		if ((i<0)||(i>=nAdj)||(j<0)||(j>=nAdj)) return;
		if (adj[i][j] == null) return;


		if (adj[i][j].getEngines() > engs) return;
		if (adj[i][j].getEngines() + adj[i][j].getCars() > cars + engs) return;

		if ( ((adj[i][j].getColor(0) != c)&&(adj[i][j].getColor(1) != c) ) &&
			(adj[i][j].getColor(0) != '*')&&(adj[i][j].getColor(1) != '*')) return;

		if ((c == adj[i][j].getColor(0) && (adj[i][j].getOwner(0) != null))) return;

		if ((c == adj[i][j].getColor(1) && (adj[i][j].getOwner(1) != null))) return;

		if (('*'==adj[i][j].getColor(0)) && (adj[i][j].getOwner(0) != null) &&
			('*'==adj[i][j].getColor(1)) && (adj[i][j].getOwner(1) != null)) return;

		
		TTRCard carCard=null, engCard=null, wildCard=null;
		
		TTRCard cards[] = p.getCards();
		wildCard = cards[TTRConst.nColors];
		
		for (int k = 0; k < TTRConst.nColors; k++) 
		{
			TTRCard card = cards[k];
			if (card.getColor()==c) 
			{
				carCard = card;
			}
			if (card.getColor()=='e')
			{
				engCard = card;
			}
		}

		if (carCard != null) 
		{
			int dec, extra = 0;
			if (carCard.getCount() < cars)
			{
				dec = carCard.getCount();
				extra = cars - dec;
			} 
			else {
				dec = cars;
			}
			
			carCard.decCount(dec);
			wildCard.decCount(extra);
		}
		
		if (engCard != null) 
		{
			int dec, extra = 0;
			if (engCard.getCount() < engs)
			{
				dec = engCard.getCount();
				extra = engs - dec;
			} 
			else dec = engs;
			engCard.decCount(dec);
			wildCard.decCount(extra);
		}
		
		p.decCars(adj[i][j].getCars() + adj[i][j].getEngines());

		//p.decCards(c, cars); p.decCards('e', engs);

		adj[i][j].setOwner(p, c);
	}

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
	public boolean claim(TTRPlayer p, int i, int j, char c, int cars, int engs)
	{
		if ((i<0)||(i>=nAdj)||(j<0)||(j>=nAdj)) return false;
		if (adj[i][j]==null) return false;

		if (adj[i][j].getEngines() > engs) return false;
		if (adj[i][j].getEngines() + adj[i][j].getCars() >
			cars + engs) return false;

		if ((adj[i][j].getColor(0) != c)&&(adj[i][j].getColor(1) != c))
			return false;
		if ((c==adj[i][j].getColor(0) && (adj[i][j].getOwner(0)!=null)))
			return false;
		if ((c==adj[i][j].getColor(1) && (adj[i][j].getOwner(1)!=null)))
			return false;

		TTRCard [] cards = p.getCards();
		for (int k = 0; k < TTRConst.nColors; k++) 
		{
			TTRCard card = cards[k];
			if (card.getColor()==c) {
				if (card.getCount() < cars) return false;
			}else if (card.getColor()=='e')
				if (card.getCount() < engs) return false;
		}

		TTRCard carCard = null, engCard = null;
		for (int k = 0; k < TTRConst.nColors; k++) 
		{
			TTRCard card = cards[k];
			if (card.getColor()==c) 
			{
				carCard = card;
				if (card.getCount()<cars) return false;
			}
			if (card.getColor()=='e')
			{
				engCard = card;
				if (card.getCount()<engs) return false;
			}
		}

		if (carCard!=null) carCard.decCount(cars);
		if (engCard!=null) engCard.decCount(engs);
		p.decCars(adj[i][j].getCars() + adj[i][j].getEngines());
		return true;
	}

	/**
	 * Removes a card from the open deck.
	 * @param c card color.
	 */
	public void removeCardFromOpenDeck(char c)
	{
		for (int i=0; i<openDeck.size(); i++) {
			if (openDeck.get(i).getColor() == c) {
				openDeck.remove(i);
			}
		}
	}
	
	/**
	 * Refreshes the open deck based on string parameter.
	 * @param s encodes new open deck contents.
	 */
	public void refreshOpenDeck(String s)
	{
		char buf [] = s.toCharArray();
		
		openDeck.getCards().clear();
		
		for (int i = 0; i < buf.length; i++) { 
			openDeck.add(new TTRCard(buf[i], 1));
		}
	}
	
	/**
	 * Get node by id.
	 * @param n node id.
	 * @return node reference.
	 */
	public TTRNode getNode(int n)
	{
		if ((n<nodeList.size())&&(n>=0)){
			return nodeList.get(n);
		}
		else {
			return null;
		}
	}

	/**
	 * Build station dummy.
	 * @param ttrPlayer
	 * @param parseInt
	 * @param parseInt2
	 * @param charAt
	 * @param parseInt3
	 * @param parseInt4
	 * 
	 */
	public void buildStationDummy(TTRPlayer p, int s, int d, char c, int cars, int engs) {
		System.out.println("buildStationDummy("+c+")");
		if ((s<0)||(s>=nAdj)||(d<0)||(d>=nAdj)) return;
	       
	       TTRCard cards[] = p.getCards();
	       TTRCard car_card = null, eng_card = null;
	       for (int i = 0; i < TTRConst.nColors; i++) {
	               TTRCard card = cards[i];
	               if (card.getColor() == c)  car_card = card;
	               if (card.getColor() == 'e')eng_card = card;
	       }
	       if ((car_card==null)||(eng_card==null)) return;
	       TTRCard wildCard = cards[TTRConst.nColors];

	       if (adj[s][d] == null) return;
	       if (nodeList.get(s).getPlayer() != null) return;

	       if (p.getNStations() < 1) return;
	       if (p.getStationArray().size()+1 < cars + engs) return;
	       
	       if (car_card!=null)
		   {
	               int dec = car_card.getCount(), extra = 0;
	               if (car_card.getCount() < cars){
	                       extra = cars - dec;
	               } else {
	            	   dec = cars;
	               }
	               car_card.decCount(dec); 
	               wildCard.decCount(extra);
	       }
	       
	       if (eng_card!=null)
		   {
	               int dec = eng_card.getCount(), extra = 0;
	               if (eng_card.getCount() < engs) {
	            	   extra = engs - dec;
	               } 
	               else {
	            	   dec = engs;
	               }
	               
	               eng_card.decCount(dec); 
	               wildCard.decCount(extra);
	       }

	       p.decNStations();
	       p.add(nodeList.get(s));
	       adj[s][d].buildStation(p);
	       nodeList.get(s).setEdge(adj[s][d]);
	       nodeList.get(s).setPlayer(p);
	}
}
