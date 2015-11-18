
package source;

import java.util.ArrayList;
import java.util.Random;

import datastruct.*;

/**
 * Class that attempts to play the game by using randomly selected actions.
 */
public class RandomSolution extends TTRClient {
	
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
	public RandomSolution(String user, String pass, 
			String agent, String host, int port){
		super(user, pass, agent, host, port);
	}
	
	/**
	 * Symbolic constant to draw from open deck. 
	 */
	static final int DRAW_FROM_OPEN_DECK= 0;
	
	/**
	 * Symbolic constant to draw from deck. 
	 */
	static final int DRAW_FROM_DECK		= 1;
	
	/**
	 * Symbolic constant to draw missions. 
	 */
	static final int DRAW_MISSIONS		= 2;
	
	/**
	 * Symbolic constant to claim a route. 
	 */
	static final int CLAIM_ROUTE		= 3;
	
	/**
	 * Symbolic constant to build a station. 
	 */
	static final int BUILD_STATION		= 4;
	
	/**
	 * Random number generator.
	 */
	Random myRand = new Random();
	
	
	public static int purple= 0, white	= 1, blue= 2, yellow= 3, orange	= 4, red= 5, green= 6, black= 7, engine	= 8;
	public static int [] myHand = new int[9];
	
	public static int [][] myNodes = new int[200][2];
	
	public static int prio = 0;
	public static int count = 0;
	
	public void refreshHand()
	{
		for(int i=0;i<myHand.length; i++)
			myHand[i]=0;
		
		TTRCard[] cards = myPlayer.getCards();
		
		for(int i = 0; i < cards.length; i++)
			switch (cards[i].getColor()) {
			case 'p':
				myHand[purple]+=cards[i].getCount();
				break;
			case 'w':
				myHand[white]+=cards[i].getCount();
				break;
			case 'b':
				myHand[blue]+=cards[i].getCount();
				break;
			case 'y':
				myHand[yellow]+=cards[i].getCount();
				break;
			case 'o':
				myHand[orange]+=cards[i].getCount();
				break;
			case 'r':
				myHand[red]+=cards[i].getCount();
				break;
			case 'g':
				myHand[green]+=cards[i].getCount();
				break;
			case 'n':
				myHand[black]+=cards[i].getCount();
				break;
			case 'e':
				myHand[engine]+=cards[i].getCount();
				break;
			default:
				break;
			}		
	}
	public int getHandMax()
	{
		refreshHand();
		int max = 0;
		for(int i=0; i<myHand.length - 1; i++) // excuding engines from loop
			if(myHand[i]>max)
				max = myHand[i];
		
		return max;
	}
	public ArrayList<String> getHandCards()
	{
		ArrayList<String> maxCards = new ArrayList<String>();
		int max = getHandMax();
		while(max > 0)
		{
			for(int i=0; i<myHand.length - 1; i++)	// excuding engines from loop
				if(myHand[i]==max)
					maxCards.add(Character.toString(TTRConst.colors[i]));
			max--;
		}
		return maxCards;
	}
	/**
	 * 
	 * @param m pairs of m cards
	 * @return number pairs of m cards
	 */
	public int getNCards(int m)
	{
		refreshHand();
		int n = 0;
		for(int i=0; i<myHand.length - 1; i++)	// excuding engines from loop
			if(myHand[i]==m)
				n++;
		
		return n;
	}
	public int isInOpenDeck(char c)
	{
		int n = 0;
		TTRDeck<TTRCard> cards = myMap.getOpenDeck();
		for(int i=0; i<cards.size(); i++)
			if(cards.get(i).getColor() == c)
				n++;
		
		return n;
	}
	public void printOpenDeck()
	{
		System.out.println("******OpenDeck******");
		TTRDeck<TTRCard> cards = myMap.getOpenDeck();
		for(int i=0; i<cards.size(); i++)
		{
			System.out.print(cards.get(i).getColor());
			System.out.print(" | ");
		}
		System.out.println();
	}
	public void printHand()
	{
		System.out.println("********Hand********");
		refreshHand();
		for(int i=0; i<myHand.length; i++)
			if(myHand[i] != 0)
			{
				for(int j = 0; j < myHand[i]; j++)
					System.out.print(TTRConst.colors[i]);
				System.out.print(" | ");
			}
		System.out.println();
	}
	
	public int numEnginesFromDeck()
	{
		TTRDeck<TTRCard> cards = myMap.getOpenDeck();
		int n = 0;
		for(int i=0; i<cards.size(); i++)
			if(cards.get(i).getColor() == 'e');
				n++;
		
		return n;
	}
	
	public void myDrawFromDeck()
	{
		ArrayList<String> hand = getHandCards();
		int m = getHandMax();
		char c = ' ';
		int k=0;
		while(c == ' ' && m > 0)
		{
			int max = 0;
			for(int i=0; i<getNCards(m); i++)
			{
				char temp = hand.get(i+k).charAt(0);
				int found = isInOpenDeck(temp);
				if((found > 0) && (found > max))
				{
					c = temp;
					max = m+found;
				}
			}
			k+=getNCards(m);
			m--;
		}
		printHand();
		printOpenDeck();
		if(c == ' ')
		{
			drawCardFromDeck();
			System.out.println("Drawing: blind");
		}
		else
		{
			drawCardFromOpenDeck(c);
			System.out.println("Drawing: "+c);
		}
		
		try {
			Thread.sleep(10);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void strategy(TTREdge[][] edge, int s, int d, int c)
	{
		refreshHand();
		if(prio == 2 && myHand[engine]<2)
		{
			if(isInOpenDeck('e')!=0)
			{
				drawCardFromOpenDeck('e');
				count++;
				return;
			}
			else
			{
				drawCardFromDeck();
				drawCardFromDeck();
				count++;
				return;
			}
		}
		int m = getHandMax();			// m = max number of cards with same color
		int num = m + myHand[engine];	// number of cards in hand to claim the route
		if( num < c)	// less than c
		{
			myDrawFromDeck();
			myDrawFromDeck();
			count++;
			actionPerformed = true;
		}
		else	// above c
		{
			System.out.println("******HERE********");
			ArrayList<String> cards = getHandCards();
			char color = cards.get(0).charAt(0);
			int wagons = m;
			int eng = edge[s][d].getEngines();
			int cars = edge[s][d].getCars(); // route length
			if(wagons < cars)
			{
				eng += cars - wagons;
			}
			else
			{
				wagons = cars;
			}
			System.out.println("Wagons: "+wagons+" engines: "+eng+" "+prio);
			printHand();
			int ret = claimRoute(s, d, color, wagons, eng);
			if(ret == -1)
				prio++;
			System.out.println("RET: "+ret);
			if (ret>=0) {
				// If more cards are required
				if(ret == 0)
				{
					prio++;
				}
				else if (ret < 3)
				{
					System.out.println("Wagons: "+wagons+" eng: "+eng+" hand: "+m+" ret: "+ret);
					if(m <= cars)
					{
						System.out.println("Claimed1: "+wagons+" "+(eng+ret));
						claimRoute(s, d, color, wagons, eng+ret);
						prio++;
					}
					else
					{
						int w = m - cars;
						int e = 0;
						if(ret < w)
							w = ret;
						else
							e = ret - w;
						
						System.out.println("Claimed2: "+(wagons+w)+" "+(eng+e));
						claimRoute(s, d, color, wagons+w, eng+e);
						prio++;
					}
				}
				else
					while ((ret != 0)&&(play)) 
						ret = claimPass();
				
					
				actionPerformed = true;
			}
		}
	}
	
	/**
	 * Action performed flag.
	 */
	boolean actionPerformed;
	
	public int getIndex(String s)
	{
		
		for(int i = 0; i < nodes.size(); i++)
			if(nodes.get(i).getName().equals(s)) return i;
		return -1;
	}
	
	int r[][] =new int[200][2];
	public static boolean isInit =false;
	
	public void initRoutes()
	{
		 nodes = myMap.getNodeList();

			
			for(int i = 0; i < nodes.size(); i++)
				if(nodes.get(i).getName().equals("stockholm"))
					stockholm = i;
				else if(nodes.get(i).getName().equals("petrograd"))
					petrograd = i;
				else if(nodes.get(i).getName().equals("moskva"))
					moskva = i;
				else if(nodes.get(i).getName().equals("kharkov"))
					kharkov = i;
				else if(nodes.get(i).getName().equals("kyiv"))
					kyiv = i;
				else if(nodes.get(i).getName().equals("budapest"))
					budapest = i;
				else if(nodes.get(i).getName().equals("palermo"))
					palermo = i;
				else if(nodes.get(i).getName().equals("smyrna"))
					smyrna = i;			

					r[0][0]=getIndex("petrograd");
					r[0][1]=getIndex("moskva");
					
					r[1][0]=getIndex("moskva");
					r[1][1]=getIndex("kharkov");
					
					r[2][0]=getIndex("kharkov");
					r[2][1]=getIndex("kyiv");
					
					r[3][0]=getIndex("petrograd");
					r[3][1]=getIndex("wilno");
					
					r[4][0]=getIndex("wilno");
					r[4][1]=getIndex("riga");
					
					r[5][0]=getIndex("petrograd");
					r[5][1]=getIndex("riga");
					
					r[6][0]=getIndex("kyiv");
					r[6][1]=getIndex("warszawa");
					
					r[7][0]=getIndex("warszawa");
					r[7][1]=getIndex("berlin");
					
					r[8][0]=getIndex("warszawa");
					r[8][1]=getIndex("berlin");
					
					r[9][0]=getIndex("warszawa");
					r[9][1]=getIndex("wien");
					
					r[10][0]=getIndex("paris");
					r[10][1]=getIndex("pamplona");
					
					r[11][0]=getIndex("paris");
					r[11][1]=getIndex("pamplona");
					
					r[12][0]=getIndex("pamplona");
					r[12][1]=getIndex("marseille");
					
					r[13][0]=getIndex("pamplona");
					r[13][1]=getIndex("brest");	
					
					r[14][0]=getIndex("edinburgh");
					r[14][1]=getIndex("london");
					
					r[15][0]=getIndex("edinburgh");
					r[15][1]=getIndex("london");
					
					r[16][0]=getIndex("sarajevo");
					r[16][1]=getIndex("athina");
					
					r[17][0]=getIndex("bucuresti");
					r[17][1]=getIndex("sevastopol");
					
					r[18][0]=getIndex("sevastopol");
					r[18][1]=getIndex("rostov");
					
					r[19][0]=getIndex("kyiv");
					r[19][1]=getIndex("bucuresti");
					
					r[20][0]=getIndex("danzig");
					r[20][1]=getIndex("berlin");
					
					r[21][0]=getIndex("paris");
					r[21][1]=getIndex("marseille");
					
					r[22][0]=getIndex("barcelona");
					r[22][1]=getIndex("marseille");
					
					r[23][0]=getIndex("stockholm");
					r[23][1]=getIndex("kobenhavn");	
					
					r[24][0]=getIndex("stockholm");
					r[24][1]=getIndex("kobenhavn");
					
					r[25][0]=getIndex("amsterdam");
					r[25][1]=getIndex("essen");
					
					r[26][0]=getIndex("frankfurt");
					r[26][1]=getIndex("paris");
					
					r[27][0]=getIndex("frankfurt");
					r[27][1]=getIndex("paris");
					
					r[28][0]=getIndex("frankfurt");
					r[28][1]=getIndex("berlin");
					
					r[29][0]=getIndex("frankfurt");
					r[29][1]=getIndex("berlin");
					
					r[30][0]=getIndex("wien");
					r[30][1]=getIndex("berlin");
					
					r[31][0]=getIndex("wien");
					r[31][1]=getIndex("munchen");
					
					r[32][0]=getIndex("wilno");
					r[32][1]=getIndex("smolensk");
					
					r[33][0]=getIndex("smolensk");
					r[33][1]=getIndex("kyiv");	
					
					r[34][0]=getIndex("sofia");
					r[34][1]=getIndex("athina");
					
					r[35][0]=getIndex("sofia");
					r[35][1]=getIndex("constantinople");
					
					r[36][0]=getIndex("constantinople");
					r[36][1]=getIndex("bucuresti");
					
					r[37][0]=getIndex("budapest");
					r[37][1]=getIndex("sarajevo");
					
					r[38][0]=getIndex("brest");
					r[38][1]=getIndex("paris");
					
					r[39][0]=getIndex("sarajevo");
					r[39][1]=getIndex("zagrab");
					
					r[40][0]=getIndex("madrid");
					r[40][1]=getIndex("lisboa");
					
					r[41][0]=getIndex("madrid");
					r[41][1]=getIndex("cadiz");
					
					r[42][0]=getIndex("riga");
					r[42][1]=getIndex("danzig");
					
					r[43][0]=getIndex("kharkov");
					r[43][1]=getIndex("rostov");	
					
					r[44][0]=getIndex("rostov");
					r[44][1]=getIndex("sochi");
					
					r[45][0]=getIndex("berlin");
					r[45][1]=getIndex("essen");
					
					r[46][0]=getIndex("essen");
					r[46][1]=getIndex("frankfurt");
					
					r[47][0]=getIndex("frankfurt");
					r[47][1]=getIndex("amsterdam");
					
					r[48][0]=getIndex("frankfurt");
					r[48][1]=getIndex("bruxelles");
					
					r[49][0]=getIndex("bruxelles");
					r[49][1]=getIndex("paris");
					
					r[50][0]=getIndex("bruxelles");
					r[50][1]=getIndex("paris");
					
					r[51][0]=getIndex("dieppe");
					r[51][1]=getIndex("bruxelles");
					
					r[52][0]=getIndex("dieppe");
					r[52][1]=getIndex("brest");
					
					r[53][0]=getIndex("barcelona");
					r[53][1]=getIndex("madrid");	
					
					r[54][0]=getIndex("madrid");
					r[54][1]=getIndex("cadiz");
					
					r[55][0]=getIndex("venezia");
					r[55][1]=getIndex("zagrab");
					
					r[56][0]=getIndex("wien");
					r[56][1]=getIndex("zagrab");
					
					r[57][0]=getIndex("zagrab");
					r[57][1]=getIndex("budapest");
					
					r[58][0]=getIndex("moskva");
					r[58][1]=getIndex("smolensk");
					
					r[59][0]=getIndex("dieppe");
					r[59][1]=getIndex("paris");
					
					r[60][0]=getIndex("wien");
					r[60][1]=getIndex("budapest");
					
					r[61][0]=getIndex("wien");
					r[61][1]=getIndex("budapest");		
					
					r[62][0]=getIndex("bruxelles");
					r[62][1]=getIndex("amsterdam");

	}
	
	private ArrayList<TTRNode> nodes;
	int stockholm = 0;
	int petrograd = 0;
	int moskva = 0;
	int kharkov = 0;
	int kyiv = 0;
	int budapest = 0;
	int palermo = 0;;
	int smyrna = 0;
	
	public void myMethod() 
	{
		// Set action performed flag to false as we have not acted this turn.
		actionPerformed = false;
		
		//ArrayList<TTRNode> nodes = myMap.getNodeList();
		TTREdge[][] edge = myMap.getAdj();
		 nodes = myMap.getNodeList();
		 if (!firstMissions && isInit==false)
		 {
			 isInit=true;
			 initRoutes();
		 }
		 
		// If (firstMissions==true) reject last received mission.
		if (firstMissions) discardHighestMission();
		else 
		
		// As long as we have not performed an action, and it is our turn, and we are 
			// still playing, attempt to choose an action to perform.
		//while ((!actionPerformed)&&(play)&&(myTurn))
		if ((!actionPerformed)&&(play)&&(myTurn))	
		{

		//	System.out.println(stock.getValue()+" "+budap.getValue()+" "+paler.getValue());
			System.out.println(myPlayer.remainingTime);
			if(isLastRound())
				System.out.println("Count: "+count+" cars: "+myPlayer.getCars());
			
			switch (prio) {
			case 0:
				strategy(edge, stockholm, petrograd, 10);
				break;
			case 1:
				strategy(edge, budapest, kyiv, 8);
				break;
			case 2:
				strategy(edge, palermo, smyrna, 4);
				break;
			default:
				if(getHandMax() < 4)
				{
					myDrawFromDeck();
					myDrawFromDeck();
				}
				else
				{
					attemptToClaimRoute();
				}
				break;
			}
			
			
						
			System.out.println();
		}
	}
	
	/**
	 * Discards last received mission.
	 */
	/** <pre>
	 public void discardLastMission(){
		int ret = 0;
		// Get mission array.
		ArrayList<TTRMission> m = myPlayer.getMissionArray();
		// While error code != 1 and we are still playing
		while ((ret != 1)&&(play)){
			// Attempt to discard mission.
			ret = rejectMissions(m.get(m.size()-1), null);
			// Set action performed flag to true.
			actionPerformed = true;
		}
	} </pre> 
	 */

	private void discardHighestMission() {
		int ret = 0,pos = 1;
		// Get mission array.
		ArrayList<TTRMission> m = myPlayer.getMissionArray();
		int max = m.get(1).getValue();
		for(int i = 1; i < m.size(); i++)
		{
			if(m.get(i).getValue() > max)
			{
				max = m.get(i).getValue();
				pos = i;
			}
		}
		// While error code != 1 and we are still playing
		while ((ret != 2)&&(play)){
			// Attempt to discard mission.
			ret = rejectMissions(m.get(0), m.get(pos));
			// Set action performed flag to true.
			actionPerformed = true;
		}
	}

	public void discardLastMission(){
		int ret = 0;
		// Get mission array.
		ArrayList<TTRMission> m = myPlayer.getMissionArray();
		// While error code != 1 and we are still playing
		while ((ret != 1)&&(play)){
			// Attempt to discard mission.
			ret = rejectMissions(m.get(m.size()-1), null);
			// Set action performed flag to true.
			actionPerformed = true;
		}
	}
	
	/**
	 * Attempt to draw from deck.
	 */
	/** <pre>
	 public void attemptToDrawFromDeck(){
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
	} </pre>
	 */
	
	public void attemptToDrawFromDeck(){
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
	}
	
	/**
	 * Attempt to draw missions. Keeps them all.
	 */
	/** <pre>
	 public void attemptToDrawFromDeck(){
		// Draw the card.
		int ret = drawCardFromDeck();
		if ((ret==1)||(ret==0))
		{
			// If successful, try to draw another card from the blind deck.
			int ret2 = 0;
			// Keep trying until successful.
			while ((ret2 != 1)&&(play))
				ret2 = drawCardFromDeck();
			// Set actionPerformed flag to true.
			actionPerformed = true;
		}
	} </pre>
	 */
	public void attemptToDrawMissions(){
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
	}
	
	/**
	 * Attempt to claim a route.
	 */
	/** <pre>
	 public void attemptToClaimRoute(){
		// Initialize edge to claim.
		TTREdge edge = null; 
		// Get number of nodes on map.
		int n = myMap.getNAdj();
		// Initialize source and destination node ids.
		int s=0, d=0;
		// As long as the edge we chose is null, randomly pick a different edge.
		while (edge == null)
		{
			s = myRand.nextInt(n); 
			d = myRand.nextInt(n);
			edge = myMap.getAdj()[s][d];
		}
		// Get edge color.
		char color = edge.getColor(myRand.nextInt(2));
		// Get out player's cards in hand.
		TTRCard [] myCards = myPlayer.getCards();
		// Get number of cars and engines required to claim route.
		int cars = edge.getCars(), engs = edge.getEngines();
		// Search for cards that match the edge's color.
		for (int i=0; i<myCards.length-1; i++)
		{
			if ((myCards[i].getColor() == color) || (color == '*'))
			{
				TTRCard card = myCards[i];
				if (card.getCount() < cars)
				{
					// Figure out how to pay for edge.
					engs += cars - card.getCount();
					cars = card.getCount();
				}
				color = card.getColor();
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
	} </pre>
	 */
	public void attemptToClaimRoute(){
		if ((!actionPerformed)&&(play)&&(myTurn)){
		ArrayList<String> hand = getHandCards();
		char c;
		for(int j=0; j<hand.size(); j++)
		{
			c = hand.get(j).charAt(0);
			for(int i=0; i<63; i++)
			{
				//System.out.println(r[i][0]+ "  "+ r[i][1]);
				TTREdge edge = myMap.getAdj()[ r[i][0] ][ r[i][1] ];
				System.out.println(edge.getNodeA().getName() + " " + edge.getNodeB().getName() + " "+edge.getCars());
				char color = edge.getColor(1);
				if(color == '0')
				{
					color = edge.getColor(0);
				}
				if(c == color || color == '*')
				{
					int ret = claimRoute(r[i][0], r[i][1], c, edge.getCars(), 0);	
					if (ret==(-3)) return;
					if(ret == 0) { System.out.println(edge.getNodeA().getName()); printHand(); actionPerformed = true; return;}
				}
			}
		}
			
	}
//	claimRoute(node1, node2, cardColor, wagons, engines);
	
	}
	
	/**
	 * Attempt to build a station.
	 */
	/** <pre>
	 public void attemptToBuildStation(){
		// If we have no stations left, move on
		if (myPlayer.getNStations() == 0) return;
		// Initialize edge to build station on.
		TTREdge edge = null; 
		// Get number of nodes on map.
		int n = myMap.getNAdj();
		// Initialize source and destination nodes.
		int s=0, d=0;
		// Randomly pick an edge.
		while (edge == null)
		{
			s = myRand.nextInt(n); 
			d = myRand.nextInt(n);
			edge = myMap.getAdj()[s][d];
		}
		// Get number of required cards to build station.
		int req = 4 - myPlayer.getNStations();
		char color = '0'; 
		// Get first card in card array to have enough cards to build station.
		TTRCard [] myCards = myPlayer.getCards();
		for (int i=0; i<myCards.length; i++)
		{
			if (myCards[i].getCount() >= req)
			{
				color = myCards[i].getColor();
				break;
			}
		}
		// Attempt to build station.
		int ret = buildStation(s, d, color, req, 0);
		// If successful, set actionPerformed flag to true.
		if (ret==1) actionPerformed = true;
	} </pre>
	 */
	public void attemptToBuildStation(){
		// If we have no stations left, move on
		if (myPlayer.getNStations() == 0) return;
		// Initialize edge to build station on.
		TTREdge edge = null; 
		// Get number of nodes on map.
		int n = myMap.getNAdj();
		// Initialize source and destination nodes.
		int s=0, d=0;
		// Randomly pick an edge.
		while (edge == null)
		{
			s = myRand.nextInt(n); 
			d = myRand.nextInt(n);
			edge = myMap.getAdj()[s][d];
		}
		// Get number of required cards to build station.
		int req = 4 - myPlayer.getNStations();
		char color = '0'; 
		// Get first card in card array to have enough cards to build station.
		TTRCard [] myCards = myPlayer.getCards();
		for (int i=0; i<myCards.length-1; i++)
		{
			if (myCards[i].getCount() >= req)
			{
				color = myCards[i].getColor();
				break;
			}
		}
		// Attempt to build station.
		int ret = buildStation(s, d, color, req, 0);
		// If successful, set actionPerformed flag to true.
		if (ret==1) actionPerformed = true;
	}
}
