
#ifndef TTRClientH
#define TTRClientH

#pragma warning (disable : 4996)

#include "String.h"
#include "Mutex.h"
#include "TTRMap.h"
#include "TTRPlayer.h"
#include "TTRClientThread.h"

#include <list>

/**
 * Abstract solution class.
 * To be extended in order to create a solution.  
 */

enum
{
    SUCCESS	    =  0,
    WC          = -1,
    WD          = -2,
    RC          = -3,
    UC          = -4,
    SEND_ERR    = -5,
    NO_RESP     = -6,
    UNK_ERR     = -7
};

class TTRClient : public TTRClientThread
{
private:

	static const int MAX_TIME;

	Mutex sync;

	std::list<String> commandQueue;

	/**
	 * User name.
	 */
	String  user; 
	
	/**
	 * Password.
	 */
	String  pass; 
	
	/**
	 * Agent name.
	 */
	String  agent;

	/**
	 * True if authenticated with server, false otherwise.
	 */
	bool authenticated; 
	
	/**
	 * True if last round, false otherwise.
	 */
	bool lastRound;

	/**
	 * The game round
	 */
	int gameRound;

	/**
	 * Internal error code.
	 */
	int	resultCode; 
	
	/**
	 * Player id.
	 */
	int playerNumber;

	/**
	 * Time left.
	 */
	long maxTime;
	
	/**
	 * Number of games available.
	 */
	int games;
	
	/**
	 * Game number.
	 */
	int gameNumber;

	/**
	 * True if in a game, false otherwise.
	 */
	bool inGame;
	
	String  errorMessage;

	void setCommand(String param);

	bool compareCommand(String param);

	String getCommand(void);

	String waitForResponse(void);

	/**
	 * Send a message to the server.
	 * @param message message to send.
	 * @return true if successful, false otherwise.
	 */
	bool sendAMessage(String  message);

protected:

	/**
	 * My turn flag. True if my turn, false otherwise.
	 */
	bool myTurn;
	
	/**
	 * Play flag.
	 */
	bool play;

	/**
	 * First missions flag.
	 */
	bool firstMissions;

	/**
	 * List of players in game with us.
	 */
	ArrayList<TTRPlayer> *playerList;
	
	/**
	 * Reference to agent's TTRPlayer object.
	 */
	TTRPlayer *myPlayer;
	
	/**
	 * Reference to current map.
	 */
	TTRMap *myMap;

public:

	/**
	 * Constructor.
	 * @warning does nothing, use at own risk.
	 */
	TTRClient();
	
	/**
	 * Constructor.
	 * Initializes everything.
	 * @param user user name.
	 * @param pass password.
	 * @param agent agent name.
	 * @param name thread name.
	 * @param host server address.
	 * @param port server port.
	 */
	TTRClient(char *user, char *pass, 
		char *agent, char *host, int port);
	
	/**
	 * Set default connection to 127.0.0.1 : 666.
	 */
	void setDefaultConnection(void);

	/**
	 * Log received command and use decode interface to handle.
	 */
	void commandHandler(String  cmd);

	/**
	 * Handle disconnect.
	 */
	void onDisconnect(void);

	/**
	 * @warning unused, does nothing.
	 */
	void onStatus(void);
	
	/**
	 * Reset object values.
	 */
	void reset(void);
	
	/**
	 * Attempt to authenticate with server. 
	 */
	void authenticate(void);
	
	/**
	 * Attempt to join a game.
	 */
	void join(void);
	
	/**
	 * Client main method.
	 * If in a game and myTurn, call myMethod.
	 * If connected and not in a game and games are left, attempt to join a game.
	 * If not connected attempt to reconnect.
	 */
	void go(void);

	/**
	 * Log string to file.
	 * Opens a PrintWriter to file and appends a String.
	 * @param s string to be logged.
	 * @param fileName log file name.
	 */
	void log(char *s, char *fileName);
	
	/**
	 * Decode a received message.
	 * @param Message message to decode.
	 */
	void decode(String  message);
	
	/**
	 * Send a command to the server. Sets resultCode.
	 * @param cmd command to send.
	 * @return last received command from server.
	 */
	String  sendACommand(String  cmd);

	/**
	 * Number of mission cards in mission deck;
	 * @author Matei Tenea
	 */
	int nMissionsInDeck(void);
	
	/**
	 * Number of cards in deck;
	 */
	int nCardsInDeck(void);

	/**
	 * Attempt to draw a card from deck.
	 * @return 
	 * 1 on success
	 * 0 on success but no cards left
	 * returns < 0 on fail
		 * -1 on Wrong Content
		 * -2 on Wrong Demand
		 * -3 on Rejected command (not your turn)
		 * -4 on Unknown command
		 * -5 on failed to send message
		 * -6 no answer from server
		 * -7 if we have a wrong answer
	 */
	int drawCardFromDeck(void);
	
	/**
	 * Attempt to draw a card from open deck.
	 * @param cardColor desired card color.
	 * @return 
	 * 1 on success
	 * 0 on success but no cards left
		 * < 0 on fail
		 * -1 on Wrong Content
		 * -2 on Wrong Demand
		 * -3 on Rejected command (not your turn)
		 * -4 on Unknown command
	 */
	int drawCardFromOpenDeck(char color);

	/**
	 * Attempt to draw missions.
	 * @return 
	 * the number of missions received on success
	 * 0 if there are no other missions left to request (DMX) (a different command can be sent)
	 * < 0 on fail
		 * -1 on Wrong Content
		 * -2 on Wrong Demand
		 * -3 on Rejected command (not your turn)
		 * -4 on Unknown command
	 */
	int drawMissions(void);

	/**
	 * Tell server we are keeping all missions.
	 * @return 
	 * 0 on success 
	 * < 0 on fail
	 * -1 on Wrong Content
	 * -2 on Wrong Demand
	 * -3 on Rejected command (not your turn)
	 * -4 on Unknown command
	 */
	int keepAllMissions(void);
	
	/**
	 * Tell server we are rejecting mission1 and mission2 (may be null).
	 * @param mission1 first mission to reject.
	 * @param mission2 second mission to reject.
	 * @return 
		2 on two missions rejected
		1 on one mission rejected
		0 on 0 missions rejected
		< 0 on fail
		-1 on Wrong Content
		-2 on Wrong Demand
		-3 on Rejected command (not your turn)
		-4 on Unknown command
	 */
	int rejectMissions(TTRMission *mission1, TTRMission *mission2);
	
	/**
	 * Attempt to claim a route.
	 * @param node1 source node.
	 * @param node2 destination node.
	 * @param cardColor card color to attempt claim.
	 * @param wagons number of cars to attempt claim.
	 * @param engines number of engines to attempt claim.
	 * @return 
		3 if three cards are required
		2 if two cards are required
		1 if one card is required
		0 on success
		< 0 on fail
		-1 on Wrong Content
		-2 on Wrong Demand
		-3 on Rejected command (not your turn)
		-4 on Unknown command
	 */
	int claimRoute(int s, int d, char color, int cars, int engs);
	
	/**
	 * Pass on a failed claim.
	 * @return 
	 	0 on success
		< 0 on fail
		-1 on Wrong Content
		-2 on Wrong Demand
		-3 on Rejected command (not your turn)
		-4 on Unknown command
	 */
	int claimPass(void);
	
	/**
	 * Attempt to build a station.
	 * @param initialNode source node.
	 * @param connectionNode destination node.
	 * @param cardColor card color to attempt build.
	 * @param wagons number of wagons of card color to attempt build.
	 * @param engines number of engnese to attept build.
	 * @return
	 	0 on success
		< 0 on fail
		-1 on Wrong Content
		-2 on Wrong Demand
		-3 on Rejected command (not your turn)
		-4 on Unknown command
	 */
	int buildStation(int s, int d, char color, int cars, int engs);

	/**
	 * Getter method for last round member.
	 * @return true if last round, false otherwise.
	 */
	bool isLastRound(void);
	
	/**
	 * Abstract method to be implemented in order to develop a solution.
	 */
	virtual void myMethod(void)=0;

	/**
	 * Destructor.
	 */
	virtual ~TTRClient(void);
};

#endif
