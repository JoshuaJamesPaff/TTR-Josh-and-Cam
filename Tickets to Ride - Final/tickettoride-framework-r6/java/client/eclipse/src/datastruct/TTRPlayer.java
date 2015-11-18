
package datastruct;

import java.util.ArrayList;

/**
 * Player class.
 * Encodes all information about a player.
 */
public class TTRPlayer {
	
	public boolean firstRun = true;
	public int nInitialMissions = 3;

	/**
	 * Reference to the game that the player is currently playing in.
	 */
	TTRMap currentGame = null;
	
	/**
	 * Player's total score.
	 */
	private int score = 0; 
	
	/**
	 * Points scored by claiming edges.
	 */
	private int edgeScore = 0;
	
	/**
	 * Points scored by completing missions.
	 */
	private int missionScore = 0;
	
	/**
	 * Points scored by not using stations.
	 */
	private int stationScore = 0;
	
	/**
	 * Longest continuous route owned by player length. 
	 */
	private int longestRoute = 0;
	
	/**
	 * Number of cars player has remaining.
	 */
	private int cars = 45;
	
	/**
	 * Number of stations player has remaining.
	 */
	private int nStations = 3;
	
	/**
	 * Player name.
	 */
	private String name;
	
	/**
	 * Team name.
	 */
	private String team;
	
	/**
	 * List of missions accepted by player.
	 */
	private ArrayList<TTRMission> missions = new ArrayList<TTRMission>();
	
	/**
	 * List of stations built by player.
	 */
	private ArrayList<TTRNode> stations = new ArrayList<TTRNode>();
	
	/**
	 * Player's hand.
	 */
	private TTRCard [] cards = new TTRCard [TTRConst.nColors + 1];
	
	/**
	 * Game flag. True if player is currently in a game.
	 */
	public boolean 	inGame 		= false; 
	
	/**
	 * Ready flag. True if player is ready. 
	 */
	public boolean 	ready 		= false;
	
	/**
	 * Turn flag. True if it is player's turn.
	 */
	public boolean 	myTurn 		= false;
	
	/**
	 * Longest path flag. True if player owns longest path.
	 */
	public boolean 	longestPath = false;
	
	/**
	 * Quantifies player's remaining time (int milliseconds).
	 */
	public int 		remainingTime;
	
	/**
	 * Number of missions player has accepted.
	 */
	public int 		nMissions;
	
	/**
	 * Last command received by player.
	 */
	public String 	lastCommand;
	
	/**
	 * Constructor.
	 * @param name agent name
	 * @param team team name
	 */
	public TTRPlayer(String name, String team)
	{
		this.name = new String(name);
		this.team = new String(team);
		
		for (int i = 0; i < TTRConst.nColors; i++)
			cards[i] = new TTRCard(TTRConst.colors[i], 0);
		cards[TTRConst.nColors] = new TTRCard('*', 4);
	}
	
	/**
	 * Resets all game related objects.
	 */
	public void clearAll()
	{
		score = 0; cars = 45;
		nStations = 3;
		missions = new ArrayList<TTRMission>();
		stations = new ArrayList<TTRNode>();
		
		cards = new TTRCard [TTRConst.nColors+1];
		for (int i=0; i<TTRConst.nColors+1; i++)
			cards[i] = new TTRCard(TTRConst.colors[i], 0);
		
		inGame = ready = myTurn = false;

		edgeScore = missionScore = longestRoute = stationScore= 0;

		currentGame = null; longestPath = false;
	}
	
	/**
	 * Setter method for currentGame member.
	 * @param game current game.
	 */
	public void  setCurrentGame(TTRMap game)
	{
		game.nPlayers++; currentGame = game;
	}
	
	/**
	 * Getter method for currentGame member.
	 * @return currentGame reference.
	 */
	public TTRMap getCurrentGame()
	{
		return currentGame;
	}

	/**
	 * Getter method for cars member.
	 * @return number of cars player has remaining.
	 */
	public int  getCars()
	{
		return cars;
	}
	
	/**
	 * Decrement number of available cars.
	 * @param d decrement value.
	 */
	public void decCars(int d)
	{
		cars -= d;
	}
	
	/**
	 * Decrement number of available cards.
	 * @param c card color.
	 * @param n count
	 */
	public void decCards(char c, int n){
		for (int i=0; i<TTRConst.nColors; i++)
			if (cards[i].getColor() == c){
				if (cards[i].decCount(n) < 0){
					cards[TTRConst.nColors].incCount(cards[i].getCount());
					cards[i].decCount(cards[i].getCount());
				}
			}
	}

	/**
	 * Get number of stations.
	 * @return number of stations.
	 */
	public int getNStations()	
	{
		return nStations;
	}
	
	/**
	 * Get player name.
	 * @return player name.
	 */
	public String getName()	
	{
		return name;
	}
	
	/**
	 * Get team name.
	 * @return team name.
	 */
	public String getTeam()	
	{
		return team;
	}

	/**
	 * Get missions.
	 * @return mission list.
	 */
	public ArrayList<TTRMission> getMissionArray()	
	{
		return missions;
	}

	/**
	 * Get stations.
	 * @return station list.
	 */
	public ArrayList<TTRNode> getStationArray()	
	{
		return stations;
	}

	/**
	 * Get player's hand.
	 * @return card array.
	 */
	public TTRCard [] getCards()	
	{
		return cards;
	}
	
	/**
	 * Get player's matching color card
	 * @param c color needed
	 * @return TTRCard instance matching @color
	 */
	TTRCard getCard(char c){
		TTRCard hand[] = getCards();
		for (int i=0; i<TTRConst.nColors+1; i++)
			if (hand[i].getColor() == c)
				return hand[i];
		return null;
	}
	
	/**
	 * Add a mission.
	 * @param mission mission to add.
	 */
	public void add(TTRMission mission)
	{
		missions.add(mission);
	}
	
	/**
	 * Add card to player's hand.
	 * @param card card object to be added.
	 */
	public void add(TTRCard card)
	{
		char color = card.getColor();
		for (int i=0; i<TTRConst.nColors+1; i++)
			if (cards[i].getColor()==color)
			{
				cards[i].incCount(card.getCount());
				break;
			}
	}
	
	/**
	 * Add card to players hand.
	 * @param c card color.
	 */
	public void addCard(char c)
	{
		for (int i=0; i<TTRConst.nColors+1; i++)
			if (cards[i].getColor()==c)
			{
				cards[i].incCount(1);
				break;
			}
	}

	/**
	 * Decrement the number of stations owned by player by one.
	 */
	public void decNStations()
	{
		nStations--;
	}

	/**
	 * When a player builds a station it is added to the station list.
	 * @param station TTRNode to be added.
	 */
	public void add(TTRNode station)
	{
		stations.add(station);
	}
	
	/**
	 * Getter method for missionScore member.
	 * @return points scored by completing or failing to complete missions.
	 */
	public int getMissionScore()
	{
		return missionScore;
	}
	
	/**
	 * Increment method for member edgeScore. 
	 * @param c value to increment by.
	 */
	public void incEdgeScore(int c)	
	{
		edgeScore += c;
	}
	
	/**
	 * Getter method for edgeScore member.
	 * @return points scored by claiming edges.
	 */
	public int getEdgeScore()
	{
		return edgeScore;
	}
	
	/**
	 * Computes points scored by not spending station resources.
	 */
	public void setStationScore()
	{
		stationScore = 4 * nStations;
	}
	
	/**
	 * Getter method for stationScore member.
	 * @return points value of remaining stations at end of game.
	 */
	public int getStationScore() 
	{
		return stationScore;
	}
	
	/**
	 * Setter method for longestRoute member.
	 * @param l longest route length.
	 */
	public void setLongestRoute(int l) 	
	{
		longestRoute = l;
	}
	
	/**
	 * Getter method for longestRoute member.
	 * @return value of player's longest continuous route.
	 */
	public int  getLongestRoute()
	{
		return longestRoute;
	}
	
	/**
	 * Adds scores together into score member modified by value 'mod'.
	 * @param mod score modifier (used for longest route).
	 */
	public void setScore(int mod)
	{
		score = missionScore + edgeScore + stationScore + mod; 
	}
	
	/**
	 * Getter method for score member.
	 * @return score points value.
	 */
	public int getScore() 
	{
		return score;
	}
	
	/**
	 * Removes 'n' cards of color 'c' from players hand.
	 * @param c card color
	 * @param n card quantity
	 */
	public void removeCard(char c, int n)
	{
		for (int i=0; i<TTRConst.nColors+1; i++)
			if (cards[i].getColor() == c)
				if (cards[i].getCount() >= n)
					cards[i].decCount(n);
				else 
				{
					int tmp = n - cards[i].getCount();
					cards[i].decCount(n - tmp);
					cards[TTRConst.nColors].decCount(tmp);
				}
	}
	
	/**
	 * Removes TTRMission m from mission list.
	 * @param m mission reference.
	 */
	public void removeMission(TTRMission m)
	{
		missions.remove(m);
	}
}
