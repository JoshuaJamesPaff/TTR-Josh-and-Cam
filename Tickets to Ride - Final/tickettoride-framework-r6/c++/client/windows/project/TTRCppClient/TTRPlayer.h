
#ifndef PlayerH
#define PlayerH

#include "TTRMission.h"
#include "ArrayList.h"
#include "TTRColor.c"
#include "TTRTime.h"
#include "TTRCard.h"
#include "String.h"

class TTRMission;
class TTRNode;
class TTRMap;

/**
 * Player class.
 * Encodes all information about a player.
 */
class TTRPlayer {

private:
	
	/**
	 * Player's total score.
	 */
	int score;
	/**
	 * Points scored by claiming edges.
	 */
	int edgeScore;
		
	/**
	 * Points scored by completing missions.
	 */
	int missionScore; 
		
	/**
	 * Points scored by not using stations.
	 */
	int stationScore;
		
	/**
	 * Longest continuous route owned by player length. 
	 */
	int longestRoute;
	
	/**
	 * Number of cars player has remaining.
	 */
	int cars;		// 45
	
	/**
	 * Number of stations player has remaining.
	 */
	int nStations;	// 3
	
	/**
	 * Player name.
	 */
	String name;
	
	/**
	 * Team name.
	 */
	String team;

	/**
	 * List of missions accepted by player.
	 */
	ArrayList<TTRMission> *missions;
	
	/**
	 * List of stations built by player.
	 */
	ArrayList<TTRNode> *stations;
	
	/**
	 * Reference to the game that the player is currently playing in.
	 */
	TTRMap *currentGame;
	
	/**
	 * Player's hand.
	 */
	TTRCard **cards;

public:

	bool firstRun;
	int nInitialMissions;

	/**
	 * No idea.
	 */
	void *Client;
	
	/**
	 * Game flag. True if player is currently in a game.
	 */
	bool InGame;
	
	/**
	 * Ready flag. True if player is ready. 
	 */
	bool Ready;
	
	/**
	 * Turn flag. True if it is player's turn.
	 */
	bool MyTurn;
	
	/**
	 * Quantifies player's remaining time (int milliseconds).
	 */
	int RemainingTime;
	
	/**
	 * Last command received by player.
	 */
	String   LastCommand;
	
	/**
	 * Number of missions player has accepted.
	 */
	int nMissions;
	
	/**
	 * Longest path flag. True if player owns longest path.
	 */
	bool longestPath;

	/**
	 * Constructor.
	 * @param name agent name
	 * @param team team name
	 */
	TTRPlayer(char *name, char *team);
	
	/**
	 * Resets all game related objects.
	 */
	void clearAll(void);
	
	/**
	 * Setter method for currentGame member.
	 * @param game current game.
	 */
	void  setCurrentGame(TTRMap *game);
	
	/**
	 * Getter method for currentGame member.
	 * @return currentGame reference.
	 */
	TTRMap *getCurrentGame(void);

	/**
	 * Getter method for cars member.
	 * @return number of cars player has remaining.
	 */
	int  getCars(void);
	
	/**
	 * Decrement number of available cars.
	 * @param d decrement value.
	 */
	void decCars(int d);

	/**
	 * Decrement number of available cards.
	 * @param c card color.
	 * @param n count
	 */
	void decCards(char c, int n);

	/**
	 * Get number of stations.
	 * @return number of stations.
	 */
	int getNStations(void);

	/**
	 * Get player name.
	 * @return player name.
	 */
	String getName(void);

	/**
	 * Get team name.
	 * @return team name.
	 */
	String getTeam(void);

	/**
	 * Get missions.
	 * @return mission list.
	 */
	ArrayList<TTRMission> *getMissionArray(void);

	
	TTRMission **getMissions(void);

	/**
	 * Get stations.
	 * @return station list.
	 */
	ArrayList<TTRNode> *getStationArray(void);

	/**
	 * Get player's hand.
	 * @return card array.
	 */
	TTRCard **getCards(void);
	
	/**
	 * Get player's matching color card
	 * @param color color neede
	 * @return TTRCard instance matching @color
	 */
	TTRCard *getCard(char color);
	
	/**
	 * Add a mission.
	 * @param mission mission to add.
	 */
	void add(TTRMission *mission);
	
	/**
	 * Add card to player's hand.
	 * @param card card object to be added.
	 */
	void add(TTRCard *card);
	
	/**
	 * Add card to players hand.
	 * @param c card color.
	 */
	void addCard(char c);

	TTRNode **getStations(void);

	/**
	 * Decrement the number of stations owned by player by one.
	 */
	void decNStations(void);

	/**
	 * When a player builds a station it is added to the station list.
	 * @param station TTRNode to be added.
	 */
	void add(TTRNode *station);
	
	/**
	 * Getter method for missionScore member.
	 * @return points scored by completing or failing to complete missions.
	 */
	int getMissionScore(void);
	
	/**
	 * Increment method for member edgeScore. 
	 * @param c value to increment by.
	 */
	void incEdgeScore(int c);
	
	/**
	 * Getter method for edgeScore member.
	 * @return points scored by claiming edges.
	 */
	int getEdgeScore(void);
	
	/**
	 * Computes points scored by not spending station resources.
	 */
	void setStationScore(void);
	
	/**
	 * Getter method for stationScore member.
	 * @return points value of remaining stations at end of game.
	 */
	int getStationScore(void);
	
	/**
	 * Setter method for longestRoute member.
	 * @param l longest route length.
	 */
	void setLongestPath(int l);
	
	/**
	 * Getter method for longestRoute member.
	 * @return value of player's longest continuous route.
	 */
	int  getLongestPath(void);
	
	/**
	 * Adds scores together into score member modified by value 'mod'.
	 * @param mod score modifier (used for longest route).
	 */
	void setScore(int mod);
	
	/**
	 * Getter method for score member.
	 * @return score points value.
	 */
	int getScore(void);
	
	/**
	 * Removes 'n' cards of color 'c' from players hand.
	 * @param c card color
	 * @param n card quantity
	 */
	void removeCard(char c, int i);

	/**
	 * Removes TTRMission m from mission list.
	 * @param m mission reference.
	 */
	void removeMission(TTRMission *m);
	
	/**
	 * Destructor.
	 */
	~TTRPlayer(void);
};
#endif
