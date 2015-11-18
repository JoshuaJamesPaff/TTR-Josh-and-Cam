
package source;

import java.io.File;
import java.io.PrintWriter;
import java.util.ArrayList;

import datastruct.*;

/**
 * Abstract solution class.
 * To be extended in order to create a solution.  
 */
public abstract class TTRClient extends TTRClientThread {
	/**
	 * Error code for Wrong Content
	 */
	public static final int WC = -1;
	
	/**
	 * Error code for Wrong Demand
	 */
	public static final int WD = -2;
	
	/**
	 * Error code for Rejected Command - not current player's turn
	 */
	public static final int RC = -3;
	
	/**
	 * Error code for Unknown Command
	 */
	public static final int UC = -4;
	
	/**
	 * Send Message Error
	 */
	public static final int SEND_ERR = -5;
	
	/**
	 * Error code for empty response from server
	 */
	public static final int NO_RESP = -6;
	
	/**
	 * Code for operation success
	 */
	public static final int SUCCESS = 0;
	
	/**
	 * Maximum time to wait for server response  
	 */
	public static final int MAX_TIME = 12000;
	
	/**
	 * Synchronisation dummy object.
	 */
	private Object sync = new Object();
	
	/**
	 * Number of mission cards in mission deck;
	 * @author Matei Tenea
	 */
	public int nMissionsInDeck(){
		int n = 40 - playerList.size() * 3;
		for (int i=0; i<playerList.size(); i++){
			TTRPlayer p = playerList.get(i);
			n -= p.nMissions - p.nInitialMissions;
		}
		return n;
	}
	
	/**
	 * Number of cards in deck;
	 */
	public int nCardsInDeck() {
		int n = 110;
		for (int i = 0; i < playerList.size(); i++) {
			TTRPlayer p = playerList.get(i);
			TTRCard c[] = p.getCards();
			for (int j = 0; j < c.length; j++) {
				n -= c[j].getCount();
			}
		}
		
		return n;
	}
	
	/**
	 * Synchronised setter method for command member.
	 * @param param set command member to param.
	 */
	private void setCommand(String param) {
		synchronized (sync){
			//command = new String(param);
			commandQueue.add(new String(param));
		}
	}
	
	/**
	 * Synchronised getter for command member.
	 * @return command string at call.
	 */
	private String getCommand() {
		String ret = null;
		synchronized (sync) {
			//System.out.println("!!!!!!!! Before Queue is: " + commandQueue);
			if (!commandQueue.isEmpty()) {
				ret = commandQueue.remove(0);
			}
			//System.out.println("!!!!!!!! After Queue is: " + commandQueue);
		}
		return ret;
	}
	
	/**
	 * Queue for the last received commands.
	 */
	//private String	command="";
	private ArrayList<String> commandQueue = new ArrayList<String>();
	
	/**
	 * User name.
	 */
	private String	user=""; 
	
	/**
	 * Password.
	 */
	private String	pass=""; 
	
	/**
	 * Agent name.
	 */
	private String	agent="";
	
	/**
	 * My turn flag. True if my turn, false otherwise.
	 */
	protected boolean	myTurn;
	
	/**
	 * True if authenticated with server, false otherwise.
	 */
	private boolean	authenticated;
	
	/**
	 * True if last round, false otherwise.
	 */
	private boolean	lastRound;
	
	/**
	 * Internal error code.
	 */
	private int	resultCode = SUCCESS; 
	
	/**
	 * Player id.
	 */
	private int	playerNumber = -1;
	
	/**
	 * Time left.
	 */
	private long maxTime = MAX_TIME;
	
	/**
	 * Number of games available.
	 */
	private int	games = 1;
	
	/**
	 * Game number.
	 */
	//private int	gameNumber;
	
	/**
	 * Play flag.
	 */
	protected boolean	play=false; 
	
	/**
	 * True if in a game, false otherwise.
	 */
	private boolean	inGame;
	
	/**
	 * Reference to agent's TTRPlayer object.
	 */
	protected TTRPlayer myPlayer;
	
	/**
	 * Reference to current map.
	 */
	protected TTRMap myMap;
	
	/**
	 * List of players in game with us.
	 */
	protected ArrayList<TTRPlayer> playerList;
	
	/**
	 * First missions flag.
	 */
	public boolean firstMissions = true;
	
	/**
	 * Constructor.
	 * @warning does nothing, use at own risk.
	 */
	public TTRClient(){}
	
	/**
	 * Constructor.
	 * Initialises everything.
	 * @param user user name.
	 * @param pass password.
	 * @param agent agent name.
	 * @param name thread name.
	 * @param host server address.
	 * @param port server port.
	 */
	public TTRClient(String user, String pass, String agent, String host, int port)
	{
		super(agent, host, port);
		this.user  = user; this.pass  = pass; this.agent = agent;
	}
	
	/**
	 * Log received command and use decode interface to handle.
	 */
	public void commandHandler(String cmd) 
	{
		String msg = new String(cmd);
		decode (msg);
		if (!cmd.equals("HELLO")) {
			log(cmd, "commandlog.txt");
		}
	}

	/**
	 * Handle disconnect.
	 */
	public void onDisconnect() 
	{
		play = false; 
		inGame = false;
		authenticated = false;
	}

	/**
	 * onStatus.
	 * @warning unused, does nothing.
	 */
	public void onStatus() {
		// dummyPlayer
	}
	
	/**
	 * Reset object values.
	 */
	public void reset()
	{
		myMap = new TTRMap(); 
		myPlayer = null;
		playerList = new ArrayList<TTRPlayer>();
		
		synchronized(sync) {
			commandQueue.clear();
		}
		
		maxTime = MAX_TIME;
		resultCode = SUCCESS; 
		playerNumber = -1;
		myTurn = false; 
		lastRound = false;
	}
	
	/**
	 * Attempt to authenticate with server. 
	 */
	public void authenticate()
	{
		String WorkCmd = "";
		String WorkStr = "A;" + user + ";" + pass + ";" + agent; 
		
		resultCode = SUCCESS; 
		
		if(!sendAMessage(WorkStr)) { 
			resultCode = SEND_ERR; 
		}
		
		int i = 4000;
		while(i != 0) {
			try { 
				Thread.sleep((long)10);
			} 
			catch (Exception e)  {
				e.printStackTrace();
			}
			
			if(authenticated) return;
			
			i -= 10;
		}
		
		String recvMsg = getCommand();
		if (recvMsg != null) {
			WorkCmd = recvMsg;
		}
		
		if(WorkCmd.equals("")) 		resultCode =  NO_RESP;
		if(WorkCmd.equals("ANO")) 	resultCode =  WC;
		if(WorkCmd.equals("AI"))	resultCode =  WD;
		if(WorkCmd.equals("AW"))	resultCode =  RC;
		if(WorkCmd.equals("UC"))	resultCode =  UC;
	}
	
	/**
	 * Attempt to join a game.
	 */
	public void join()
	{
		sendAMessage("JOIN");
		
		while (games != 0)
		{
			// wait for server to send name of game map and then load data from it
			if(myMap.getNodeList() != null) {
				break;
			}
			
			try 
			{ 
				Thread.sleep((long)1);
			} 
			catch (Exception e) 
			{
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * Client main method.
	 * If in a game and myTurn, call myMethod.
	 * If connected and not in a game and games are left, attempt to join a game.
	 * If not connected attempt to reconnect.
	 */
	public void go() {
		while (games > 0) {
			if (connected) {
				play = true;
				if(!authenticated) authenticate();
				else {
					if(!inGame) {
						reset();	// clear all / delete
		                join();		// join a game
						inGame = true;
		                play = true;
					}
					while (play) {
						if (myTurn) {
							if (firstMissions) {
								// clear command queue of received initial cards
								synchronized (sync) {
									commandQueue.clear();
								}
							}
							
							myMethod();
							firstMissions = false;
						}
						
						try {
							Thread.sleep((long) 1);
						} catch (Exception e) {
							e.printStackTrace();
						}
					}
					
		            // GET SCORES HERE
					if(!inGame) {
						games --;
						System.out.println("Games left: " + games);
					}
					// else would mean disconnect
				}
			} 
			else {
				if(!connect()) {
					try {
						Thread.sleep((long)300);
					} 
					catch (Exception e) {
						e.printStackTrace();
					}
				} 
				else {
					authenticate();
				}
			}
		}
		
	}

	/**
	 * Log string to file.
	 * Opens a PrintWriter to file and appends a String.
	 * @param s string to be logged.
	 * @param fileName log file name.
	 */
	public void log(String s, String fileName) 
	{
		try {
			PrintWriter out = new PrintWriter(new File(fileName));
			out.append(s); out.close();
		} 
		catch (Exception e) 
		{ 
			e.printStackTrace(); 
		}
		
	}
	
	/**
	 * Decode a received message.
	 * @param Message message to decode.
	 */
	public void decode(String Message) 
	{ 
		// HERE BE BIG DRAGONS!!!
		String WorkStr, MisStr;
		int i, Pnr;
		
		//setCommand(""); // might want a CRITICAL!!! bwahahaha
		if(Message.equals("WC")){ setCommand( Message ); return; }

		if(Message.equals("WD")){ setCommand( Message ); return; }

		if(Message.equals("UC")) { setCommand( Message ); return; }

		if(Message.equals("RC")) { setCommand( Message ); return; }

		switch (Message.charAt(0)) 
		{

			case 'H':	
				// if (Message.equals("HELLO")) send hello back
				break;

			case 'A':	// Auth : AOK, ANO, AI, AM, AW
						if(Message.equals("AOK")) 
						{
							authenticated = true;
							return;
						}
	                    setCommand( Message );
				break;

			case 'D':	// Draw: DiDc DiD DiOc DiM DMX
						if (Message.equals("DMX")) {	// no mission cards left
		                	if (myTurn) {
		                		setCommand(Message);
		                	}
		                	return;
		                }
						
						Pnr = Integer.parseInt(Message.substring(1,2));
	                    Pnr --;
	                    
	                    
						switch (Message.charAt(2)) 
						{
							case 'D':
										if(Message.length() == 4) {
											// Message[3]
											playerList.get(Pnr).addCard(Message.charAt(3));
	                                        if (Pnr == playerNumber && myTurn) {
	                                        	setCommand( Message );
	                                        }
										} else {
											playerList.get(Pnr).addCard('*');
										}
								break;

							case 'O':
										playerList.get(Pnr).addCard(Message.charAt(3));
										if(playerNumber == Pnr && myTurn) { // myTurn?
		                                    setCommand( Message );
										}
								break;

							case 'M':
										if(Message.length() == 3)
										{
											// missions ++
											playerList.get(Pnr).nMissions ++; // ++
										}
										else
										{
											TTRMission TmpMis;
											i = Message.length() - 3; i /= 6;
											WorkStr = Message;
											
											while(i!=0) {
												MisStr = WorkStr.substring(WorkStr.length()-6, WorkStr.length());
												WorkStr = WorkStr.substring(0,WorkStr.length()-6);

												TmpMis = new TTRMission(myMap.getNode(Integer.parseInt(MisStr.substring(0,2))),
																		myMap.getNode(Integer.parseInt(MisStr.substring(2,4))),
																		Integer.parseInt(MisStr.substring(4,6)));
												playerList.get(Pnr).add(TmpMis);

												i--;
											}
										}
										if(playerNumber == Pnr && myTurn) {
											setCommand(Message);
										}
								break;
								
							default:
	                        ;
						}
				break;

			case 'E': 	// Reject mission Ein
						Pnr = Integer.parseInt(Message.substring(1,2));
						Pnr --;

						i = Integer.parseInt(Message.substring(2,3));
						
						TTRPlayer pTmp = playerList.get(Pnr);
						if (pTmp.firstRun){
							pTmp.firstRun = false;
							pTmp.nInitialMissions -= i;
						}
						else 
							pTmp.nMissions -= i; // missions --

						if( playerNumber == Pnr ) {
							setCommand( Message );
						}
				break;

			case 'S':	// Open deck: Sccccc
						myMap.refreshOpenDeck(Message.substring(1,6));
				break;

			case 'C':	// Claim: CiCxxjjczztt CiTxxjjczztt CiDnccc CiP .
						Pnr = Integer.parseInt(Message.substring(1,2));
						Pnr --;

						switch (Message.charAt(2))
						{
							case 'C':
										if(Pnr == playerNumber && myTurn)
										{
											myMap.claimRouteDummy(playerList.get(Pnr), Integer.parseInt(Message.substring(3,5)),
													Integer.parseInt(Message.substring(5,7)), Message.charAt(7), Integer.parseInt(Message.substring(8,10)),
													Integer.parseInt(Message.substring(10,12)));
	                                        setCommand( Message );
										}
										else
										{
											myMap.claimRouteDummy(playerList.get(Pnr), Integer.parseInt(Message.substring(3,5)),
											Integer.parseInt(Message.substring(5,7)), Message.charAt(7), Integer.parseInt(Message.substring(8,10)),
											Integer.parseInt(Message.substring(10,12)));
										}
								break;

							case 'T': // fara incercari or stuff like that!
								break;

							case 'D':
										// ccc nu inca - in the future!                                	
	                                if (Pnr == playerNumber && myTurn) {    
	                                	setCommand( Message );
	                                }
								break;

							case 'P':
									if (Pnr == playerNumber && myTurn) {
	                                    setCommand( Message );
									}
								break;

							default:
	                        ;
						}


				break;

			case 'B': 	// Build Station: Bixxyyczztt
						Pnr = Integer.parseInt(Message.substring(1,2));
						Pnr --;

						if(Pnr == playerNumber)
						{
							myMap.buildStationDummy(playerList.get(Pnr), Integer.parseInt(Message.substring(2,4)),
									Integer.parseInt(Message.substring(4,6)),Message.charAt(6), Integer.parseInt(Message.substring(7,9)),
									Integer.parseInt(Message.substring(9,11)));
							setCommand( Message );
						} else {
							myMap.buildStationDummy(playerList.get(Pnr), Integer.parseInt(Message.substring(2,4)),
							Integer.parseInt(Message.substring(4,6)),Message.charAt(6), Integer.parseInt(Message.substring(7,9)),
							Integer.parseInt(Message.substring(9,11)));
						}

				break;	

			case 'G':	// Game info: GM<myMap.xml> GI00nSTART GPni GTi<time> GIL GIE
						// GSiM<mis score> GSiE<edge score> GSiS<station score>
						// GSiT<totla score> GSiP<path score> GSiL<Longest path>
						switch (Message.charAt(1))
						{
							case 'M' : // myMap
										WorkStr = Message.substring(2, Message.length());
	                                    myMap.loadFromFile(WorkStr); // add an if and log error!
								break;

							case 'I' : // start stop
										if(Message.charAt(2)=='L') 
										{
											lastRound = true;
											return; // or break
										}

										if(Message.charAt(2)=='E') 
										{
											play = false;
											inGame = false;
											quit = true;
											return;
										}

										if(Message.indexOf("START") != -1) 
										{
											//gameNumber = Integer.parseInt(Message.substring(2,5));
	                                    	// sure hope I wont have issues!
											sendAMessage("READY");
										}
								break;

							case 'P' : // player number / order
										Pnr = Integer.parseInt(Message.substring(3,4));
										Pnr -- ;
										i = Integer.parseInt(Message.substring(2,3));
	                                    TTRPlayer newPlayer;

										for(int l=0;l<i;l++)
										{
											if(l == Pnr) 
											{
												newPlayer = new TTRPlayer(agent, user);
												myPlayer = newPlayer;
	                                            playerNumber = Pnr;
											} else { 
												newPlayer = new TTRPlayer("",""); 
											}
											playerList.add(newPlayer);
										}
								break;

							case 'T' : // turn & time
										Pnr = Integer.parseInt(Message.substring(2,3));
										Pnr --;
										i = Integer.parseInt(Message.substring(3, Message.length()));
										playerList.get(Pnr).remainingTime = i;

										if(Pnr == playerNumber) {
											myTurn = true;
										}
								break;

							case 'S' : // score info
	                                   // TO BE ADDED!!
								break;

							default: 
								break;
						}
				break;

			case 'T':	// end of turn: Ti<remaining time>
						Pnr = Integer.parseInt(Message.substring(1,2));
						Pnr --;
						i = Integer.parseInt(Message.substring(2,Message.length()));
						playerList.get(Pnr).remainingTime = i;

						if(Pnr == playerNumber) { myTurn = false; }
				break;

			default:
			;
		}
	}
	
	/**
	 * Send a message to the server.
	 * @param message message to send.
	 * @return true if successful, false otherwise.
	 */
	private boolean sendAMessage(String message) 
	{
		// might need synchronized section!
		try { 
			send(message);
		} 
		catch (Exception e) {
			return false;
		}
		return true; // success
	}
	
	/**
	 * Send a command to the server. Sets resultCode.
	 * @param cmd command to send.
	 * @return last received command from server.
	 */
	public String sendACommand(String cmd)
	{
		
		//setCommand( "" );
		if(!sendAMessage(cmd)) { 
			resultCode = SEND_ERR;
			return "";
		}
		
		return waitForResponse();
	}
	
	private String waitForResponse() {
		String workCmd = ""; 
		resultCode = SUCCESS;
		
		maxTime = MAX_TIME;
		while(play) {
			try { 
				Thread.sleep((long)1);
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			String recvMsg = getCommand();
			if (recvMsg != null) {
				workCmd = recvMsg;
				break;
			}
			else {
				maxTime -= 1;
			}
			
			if(maxTime <= 0) {
				break;
			}
		}

		if(workCmd.equals("")) 	resultCode =  NO_RESP;
		if(workCmd.equals("WC"))resultCode =  WC;
		if(workCmd.equals("WD"))resultCode =  WD;
		if(workCmd.equals("RC"))resultCode =  RC;
		if(workCmd.equals("UC"))resultCode =  UC;
		
		return workCmd;
	}
	
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
	 */
	public int drawCardFromDeck()
	{
		String workCmd = sendACommand("CDD");
		if(workCmd.equals("")) {
			return resultCode;
		}
	
		String workStr = "D";
		workStr += "" + (playerNumber + 1);
		workStr += "D";

		if(workCmd.length() != 4) return resultCode;
		if(!workStr.equals(workCmd.substring(0, 3))) return resultCode;

		if(workCmd.charAt(3) != '0') return 1;
		return 0;
	}
	
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
	public int drawCardFromOpenDeck(char cardColor)
	{
		String workStr = "CDO" + cardColor;
		String workCmd = sendACommand(workStr);

		if(workCmd.equals("")) {
			return resultCode;
		}

		workStr = "D";
		workStr += "" + (playerNumber + 1);
		workStr += "O";
	    workStr += cardColor;

		if((!workStr.equals(workCmd)) || (workCmd.length() != 4)) return resultCode;
		
		if(workCmd.charAt(3) != '0') return 1;
		return 0;
	}
	
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
	public int drawMissions()
	{
		String workStr = "CDM";
		String workCmd = sendACommand(workStr);

		if(workCmd.equals("")) {
			return resultCode;
		}

		workStr = "D";
		workStr += "" + (playerNumber + 1);
		workStr += "M";

		if(workCmd.length() < 3) return resultCode;
		if(workCmd.charAt(2) == 'X') return 0;		// if no more missions return 0
		
		if(!workStr.equals(workCmd.substring(0,3))) return resultCode;
		
		int i = workCmd.length(); 
		i-= 3; i /= 6;
		return i;
	}
	
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
	public int keepAllMissions()
	{
		String workStr = "CE";
		String workCmd = sendACommand(workStr);
		if(workCmd.equals("")) return resultCode;

		workStr = "E";
		workStr += "" + (playerNumber + 1);
		workStr += "0";

		if(workCmd.length() != 3) return resultCode;
		if(!workStr.equals(workCmd)) return resultCode;
		return 0;
	}
	
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
	public int rejectMissions(TTRMission mission1, TTRMission mission2)
	{
		if (myPlayer.firstRun){
			myPlayer.firstRun = false;
			if (mission1 != null) myPlayer.nInitialMissions--;
			if (mission2 != null) myPlayer.nInitialMissions--;
		}
			
		String workCmd, workStr = "CE"; 
		int i, k = 0;
		
		if(mission1 != null)
		{
			i = mission1.getNodeA().getId();
			if(i < 10) { 
				workStr += "0"; 
			}
			workStr += i;
			
			i = mission1.getNodeB().getId();
			if(i < 10) { 
				workStr += "0";
			}
			workStr += i;

			i = mission1.getValue();
			if(i < 10){ 
				workStr += "0";
			}
			workStr += i;
			k++;
		}

		if(mission2 != null) 
		{
			i = mission2.getNodeA().getId();
			if(i<10) { 
				workStr += "0";
			}
			workStr += i;
			
			i = mission2.getNodeB().getId();
			if(i<10) {
				workStr += "0";
			}
			workStr += i;

			i = mission2.getValue();
			if(i<10) {
				workStr += "0";
			}
			workStr += i;
			k++;		
		}

		if(workStr.equals("CE")) return keepAllMissions();
		
		workCmd = sendACommand(workStr);
		if(workCmd.equals("")) {
			return resultCode;
		}
		
		workStr = "E";
		workStr += "" + (playerNumber + 1);
		workStr += k;
		
		if (workCmd.length() != 3) return resultCode;
		if (!workStr.equals(workCmd)) return resultCode;

		if (mission1 != null) {
			myPlayer.removeMission(mission1);
		}
		
		if (mission2 != null) {
			myPlayer.removeMission(mission2);
		}
		
		return k;
	}
	
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
	public int claimRoute(int node1, int node2, char cardColor, int wagons, int engines)
	{
		String workCmd, workStr; 
		
		workStr = "CC";
		if(node1 < 10) {
			workStr += "0"; 
		}
		workStr += node1;
		
		if(node2 < 10) {
			workStr += "0";
		}
		workStr += node2;
		
		workStr += cardColor;
		
		if (wagons < 10) {
			workStr += "0";
		}
		workStr += wagons;
		
		if (engines <10) {
			workStr += "0"; 
		}
		workStr += engines;
		
		workCmd = sendACommand(workStr);
		if(workCmd.equals("")) return resultCode;

		workStr = "C" + (playerNumber + 1) + workStr.substring(1,11);
		if(workCmd.equals(workStr)) {
	        //myMap.claimRouteDummy(myPlayer, node1, node2, cardColor, wagons, engines);
	    	return 0;
		}
		
		if((workCmd.length() != 12) && (workCmd.length() != 7)) return resultCode;
		
		int i = 0;
		if(workCmd.charAt(2) == 'D') { 
			i = Integer.parseInt(workCmd.substring(3,4));
		}
		
		// i know i've successfully claimed a tunnel route - so wait for confirmation 
		// i know the server is going to send it
		if (i == 0) {
			workCmd = waitForResponse();
			
			if (!workCmd.equals(workStr)) {
				return resultCode; 
			}
		}
		
		return i;
	}
	
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
	public int claimPass()
	{
		String workStr = "CCP";
		String workCmd = sendACommand(workStr);
		
		if(workCmd.equals("")) {
			return resultCode;
		}
		
		// DiM on success!
		workStr = "C";
		workStr += "" + (playerNumber + 1);
		workStr += "P";

		if(workCmd.length()!=3) return resultCode;
		if(!workStr.equals(workCmd))  return resultCode;
		return 0;
	}
	
	/**
	 * Attempt to build a station.
	 * @param initialNode source node.
	 * @param connectionNode destination node.
	 * @param cardColor card color to attempt build.
	 * @param wagons number of wagons of card color to attempt build.
	 * @param engines number of engines to attempt build.
	 * @return
	 	0 on success
		< 0 on fail
		-1 on Wrong Content
		-2 on Wrong Demand
		-3 on Rejected command (not your turn)
		-4 on Unknown command
	 */
	public int buildStation(int initialNode, int connectionNode, char cardColor, int wagons, int engines)
	{
		String workStr = "CB";
		
		if(initialNode < 10) { 
			workStr += "0";
		}
		workStr += initialNode;
		
		if(connectionNode < 10) {
			workStr += "0";
		}
		workStr += connectionNode;
		
		workStr += cardColor;
		
		if (wagons < 10) {
			workStr += "0"; 
		}
		workStr += wagons;
		
		if (engines <10) {
			workStr += "0";
		}
		workStr += engines;
		
		String workCmd = sendACommand(workStr);
		
		if(workCmd.equals("")) {
			return resultCode;
		}

		workStr = "B" + (playerNumber + 1) + workStr.substring(2,11);
		if(workCmd.equals(workStr)) {
	        //myMap.claimRouteDummy(myPlayer, initialNode, connectionNode, cardColor, wagons, engines);
			return 0;
		}
		
		return resultCode;		
	}
	
	/**
	 * Getter method for last round member.
	 * @return true if last round, false otherwise.
	 */
	boolean isLastRound() 
	{
		return lastRound;
	}
	
	/**
	 * Abstract method to be implemented in order to develop a solution.
	 */
	public abstract void myMethod();
}
