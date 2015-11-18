

#include "TTRClient.h"

const int TTRClient::MAX_TIME = 1200;		// max time to wait for server response

int TTRClient::nMissionsInDeck(void){
	int n = 40 - playerList->size() * 3;
	for (int i=0; i<playerList->size(); i++){
		TTRPlayer *p = playerList->get(i);
		n -= p->nMissions - p->nInitialMissions;
	}
	return n;
}

int TTRClient::nCardsInDeck(void) {
	int n = 110;
	for (int i=0; i<playerList->size(); i++){
		TTRPlayer *p = playerList->get(i);
		TTRCard **c  = p->getCards();
		for (int j=0; j<nColors + 1; j++)
			n -= c[j]->getCount(); 
	}
	return n;
}

void TTRClient::setCommand(String param){
	sync.acquire();
	commandQueue.push_back(param);
	sync.release();
}

String TTRClient::getCommand(){
	String ret;
	sync.acquire();
	if (commandQueue.size())
	{
		ret = commandQueue.front();
		commandQueue.pop_front();
	}
	sync.release();
	return ret;
}

bool TTRClient::sendAMessage(String  message){
	send(message); return true;
}

TTRClient::TTRClient(){ firstMissions = true; inGame = false; gameRound = 0; }

TTRClient::TTRClient(char *user, char *pass, char *agent, 
	char *host, int port) : TTRClientThread(agent, host, port)
{
	games = 1;
	play = false;
	maxTime = MAX_TIME;
	resultCode = SUCCESS;
	authenticated = false;
	gameRound = 0;

	this->user   = user;
	this->pass   = pass;
	this->agent  = agent;
	errorMessage = "";

	playerList = (ArrayList<TTRPlayer> *)0;
	myMap = (TTRMap *)0; myPlayer = (TTRPlayer *)0;
	firstMissions = true;
	inGame = false;
}
	
/**
 * Set default connection.
 */
void TTRClient::setDefaultConnection(void)
{
	hostName = "127.0.0.1"; port = 1337;
}

/**
 * Log commands and sen them to decoder.
 */
void TTRClient::commandHandler(String  cmd)
{
	String  msg = cmd;
	decode (msg);
	if (!(cmd == "HELLO")) 
		log(cmd.data, "commandlog.txt");
}

void TTRClient::onDisconnect(void)
{
	play			= false;
	inGame			= false; 
	authenticated	= false;
}

/**
 * @warning dummyPlayer hook function
 */
void TTRClient::onStatus(void)
{
	// dummyPlayer
}
	
void TTRClient::reset(void)
{
	if(myMap != (TTRMap *)0) delete myMap;
	
	myMap = new TTRMap();

	if(playerList != (ArrayList<TTRPlayer> *)0) delete playerList;
	playerList = new ArrayList<TTRPlayer>();

	myPlayer = (TTRPlayer *)0;

	sync.acquire();
	commandQueue.clear();
	sync.release();

	maxTime = MAX_TIME;
	resultCode = SUCCESS;
	playerNumber = -1;
	myTurn = false;
	lastRound = false;
	inGame = false;
}
	
void TTRClient::authenticate(void)
{
	String  workCmd, workStr;
	int i;

	resultCode = SUCCESS;
	workCmd = "";

	workStr = "A;";
	workStr += user;
	workStr += ";";
	workStr += pass;
	workStr += ";"; 
	workStr += agent;

	if(!sendAMessage(workStr))
		return;

	i = 4000;
	while(i!=0)
	{
		if(authenticated)
			return;
		i -=10; Thread::sleep((long)10);
	}

	if(i!=0)
	{
		workCmd = getCommand();
	}

	if (workCmd == "")
		resultCode = NO_RESP;
	if (workCmd == "ANO")
		resultCode = WC;
	if (workCmd == "AI")
		resultCode = WD;
	if (workCmd == "AW")
		resultCode = RC;
	if (workCmd == "UC")
		resultCode = UC;
}
	
void TTRClient::join(void)
{
	// if i have a map loaded then I joined! ;)
	sendAMessage("JOIN");
	while(games!=0)
	{
		if(myMap->getNodeList() != (ArrayList<TTRNode> *)0) break;
		Thread::sleep((unsigned long)1);
	}

}
	
void TTRClient::go(void)
{
	while (games > 0)
	{
		if (connected)
		{
			play = true;
			if(!authenticated) authenticate();
			else
			{
				if(!inGame)
				{
					reset();	// clear all / delete
					join();		// join a game
					inGame = true;
					play = true;
				}
				while(play)
				{
					if(myTurn)
					{
						if (firstMissions)
						{
							sync.acquire();
							commandQueue.clear();
							sync.release();
						}

						myMethod();
						firstMissions = false;
					}
					Thread::sleep(1);
				}
				// GET SCORES HERE
				if(!inGame)  games --;
				// else would mean disconnect
			}
		}
	}
}

void TTRClient::log(char *s, char *fileName)
{
	FILE *f;
	fopen_s(&f, fileName, "at");
	if (f){
		fprintf(f, "%s\n", s);
		fclose(f);
	} else {
		printf("error appending '%s' to file '%s'\n", s, fileName);
	}
}
	
void TTRClient::decode(String  message)
{
	// HERE BE BIG DRAGONS!!!
	String  workStr, MisStr;
	int i, pnr;

//	setCommand( (String )""; // might want a CRITICAL!!! bwahahaha

	if(message == "WC") {setCommand(message); return;}

	if(message == "WD") {setCommand(message); return;}

	if(message == "UC") {setCommand(message); return;}

	if(message == "RC") {setCommand(message); return;}
	
	switch (message.data[0])
	{

		case 'H':	// Hello!
					// send hello back.. or not apparently
			break;

		case 'A':	// Auth : AOK, ANO, AI, AM, AW
					if(message == "AOK")
					{
						authenticated = true;
						return;
					}
					setCommand(message);
				
			break;

		case 'D':	// Draw: DiDc DiD DiOc DiM DMX
					if (message == "DMX")	// no more mission cards left
					{
						if (myTurn) 
						{
							setCommand(message);
						}
						return;
					}
					
					pnr = message.substring(1,1).toInt();
                    pnr --;

					switch (message.data[2])
					{
						case 'D':
									if(message.length() == 4)
									{
										// message[2]
										playerList->get(pnr)->addCard(message.data[3]);
										if (playerNumber == pnr && myTurn) 
										{
											setCommand(message);
										}
									}
									else
									{
										playerList->get(pnr)->addCard('*');
									}
							break;

						case 'O':
									playerList->get(pnr)->addCard(message.data[3]);

									if(playerNumber == pnr && myTurn) 
									{ 
	                                    setCommand(message);
									}
							break;

						case 'M':
									if(message.length()==3)
									{
										playerList->get(pnr)->nMissions ++; // ++
									}
									else
									{
										TTRMission *TmpMis;

										i = message.length() - 3;
										i /= 6;

										workStr = message;

										while(i!=0)
										{
											MisStr = workStr.substring(workStr.length()-6, 6);
											workStr = workStr.substring(0, workStr.length()-6);

											TmpMis = new TTRMission(myMap->getNode(MisStr.substring(0,2).toInt()),
																	myMap->getNode(MisStr.substring(2,2).toInt()),
                                                                    MisStr.substring(4,2).toInt());
											playerList->get(pnr)->add(TmpMis);

											i--;
										}
									}
									if(playerNumber == pnr && myTurn) 
									{
	                                    setCommand(message);
									}
							break;

						default:
                        ;
					}
			break;

		case 'E':{ 	// Reject mission Ein
					pnr = message.substring(1,1).toInt();
					pnr --;

					i = message.substring(2,1).toInt();

					TTRPlayer *pTmp = playerList->get(pnr);
					if (pTmp->firstRun){
						pTmp->firstRun = false;
						pTmp->nInitialMissions -= i;
					}
					else 
						pTmp->nMissions -= i; // missions --

					if(playerNumber == pnr) 
					{
						setCommand(message);
					}
			break;
		}
		case 'S':	// Open deck: Sccccc
					// REFRESH DECK HERE!
					myMap->refreshOpenDeck(message.substring(1,5));

					// command doesnt get messag :P
			break;

		case 'C':	// Claim: CiCxxjjczztt CiTxxjjczztt CiDnccc CiP .
					pnr = message.substring(1,1).toInt();
					pnr --;

					switch (message.data[2])
					{
						case 'C':
									if (playerNumber == pnr && myTurn)
									{
										/*
										if (myPlayer != playerList->get(pnr)) 
											myMap->claimRouteDummy(playerList->get(pnr),message.substring(3,2).toInt(),
										message.substring(5,2).toInt(),message.data[7], message.substring(8,2).toInt(),
										message.substring(10,2).toInt());
										setCommand(message);
										*/
										myMap->claimRouteDummy(myPlayer,message.substring(3,2).toInt(),
										message.substring(5,2).toInt(),message.data[7], message.substring(8,2).toInt(),
										message.substring(10,2).toInt());
										
                                        setCommand(message);
									}
									else
									{
										myMap->claimRouteDummy(playerList->get(pnr),message.substring(3,2).toInt(),
										message.substring(5,2).toInt(),message.data[7], message.substring(8,2).toInt(),
										message.substring(10,2).toInt());
									}
							break;

						case 'T': 
							break;

						case 'D':
								if (pnr == playerNumber && myTurn)
								{
									setCommand(message);
								}
							break;

						case 'P':
							if (pnr == playerNumber && myTurn) 
							{
								setCommand(message);
							}
							break;

						default:
							break;
					}


			break;

		case 'B': 	// Build Station: Bixxyyczztt
					pnr = message.substring(1,1).toInt();
					pnr --;

					if(pnr == playerNumber)
					{
						myMap->buildStationDummy(myPlayer,message.substring(2,2).toInt(),
						message.substring(4,2).toInt(),message.data[6], message.substring(7,2).toInt(),
						message.substring(9,2).toInt());

						setCommand(message);
					}
					else
					{
						myMap->buildStationDummy(playerList->get(pnr),message.substring(2,2).toInt(),
						message.substring(4,2).toInt(),message.data[6], message.substring(7,2).toInt(),
						message.substring(9,2).toInt());
					}

			break;	

		case 'G':	// Game info: GM<map.xml> GI00nSTART GPni GTi<time> GIL GIE
					// GSiM<mis score> GSiE<edge score> GSiS<station score>
					// GSiT<totla score> GSiP<path score> GSiL<Longest path>
					switch (message.data[1])
					{
						case 'M' : // map							
									workStr = message.substring(2, message.length()-2);
                                    myMap->loadFromFile(workStr.data); // add an if and log error!
							break;

						case 'I' : // start stop
									if(message.data[2]=='L')
									{
										lastRound = true;
										return; // or break
									}

									if(message.data[2]=='E')
									{
										play = false;
										inGame = false;
										quit = true;
										return;
									}

									if(message.pos("START")>=0)
									{
										//gameNumber = message.substring(3,3).toInt();
                                    	// sure hope I wont have issues!
										sendAMessage("READY");
									}
							break;

						case 'P' : // player number / order
									pnr = message.substring(3,1).toInt();
									pnr -- ;
									i = message.substring(2,1).toInt();
                                    TTRPlayer *NewPlayer;

									for(int l=0;l<=i;l++)
									{
										if(l == pnr)
										{
											NewPlayer = new TTRPlayer(agent.data, user.data);
											myPlayer = NewPlayer;
                                            playerNumber = pnr;
										}
										else
										{
                                            NewPlayer = new TTRPlayer("","");
										}
										playerList->add(NewPlayer);
									}
							break;

						case 'T' : // turn & time
									pnr = message.substring(2,1).toInt();
									pnr--;
									i = message.substring(3, message.length()-3).toInt();
									playerList->get(pnr)->RemainingTime = i;

									if(pnr == playerNumber) 
									{
										myTurn = true;
									}
							break;

						case 'S' : 
							break;

						default:
						;
					}
			break;

		case 'T':	// end of turn: Ti<remaining time>
					pnr = message.substring(1,1).toInt();
					pnr--;
					i = message.substring(2, message.length()-2).toInt();
					playerList->get(pnr)->RemainingTime = i;

					if(pnr == playerNumber) { myTurn = false; }
			break;

		default: break;
	}
}
	
String  TTRClient::sendACommand(String  cmd)
{
	resultCode = SUCCESS;

	if (!sendAMessage(cmd))
	{
		resultCode = SEND_ERR;
		return "";
	}

	return waitForResponse();
}

String TTRClient::waitForResponse()
{
	String workCmd = "";
	resultCode = SUCCESS;

	maxTime = MAX_TIME;
	while (play)
	{
		Thread::sleep((long)1);
		workCmd = getCommand();
		if (workCmd != "")
		{
			break;
		}
		else
		{
			maxTime -= 1;
		}

		if (maxTime <= 0) break;
	}

	if(workCmd == "")	resultCode = NO_RESP;
	if(workCmd == "WC") resultCode = WC;
	if(workCmd == "WD") resultCode = WD;
	if(workCmd == "RC") resultCode = RC;
	if(workCmd == "UC") resultCode = UC;

	return workCmd;
}

/**
 * Attempt to draw a card from deck.
 * return 
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
int TTRClient::drawCardFromDeck(void)
{
	String  workCmd, workStr;
	workCmd = sendACommand("CDD");
	if(workCmd == "")
		return resultCode;

	workStr = "D";
	workStr += playerNumber + 1;
	workStr += "D";

	if(workCmd.length()!=4) return resultCode;

	if(workStr != workCmd.substring(0, 3)) return resultCode;

	if(workCmd.data[3] != '0') return 1;
	return 0;
}

/**
 * Attempt to draw a card from open deck.
 * param cardColor desired card color.
 * return 
 * 1 on success
 * 0 on success but no cards left
	 * < 0 on fail
	 * -1 on Wrong Content
	 * -2 on Wrong Demand
	 * -3 on Rejected command (not your turn)
	 * -4 on Unknown command
 */
int TTRClient::drawCardFromOpenDeck(char color)
{
	String  workCmd, workStr;
	workStr = "CDO";
	workStr += color;
	workCmd = sendACommand(workStr);

	if(workCmd == "")
		return resultCode;

	workStr = "D";
	workStr += playerNumber + 1;
	workStr += "O";
    workStr += color;

	if((workStr != workCmd) || (workCmd.length()!=4)) return resultCode;
	if(workCmd.data[3] != '0') return 1;
	return 0;
}

/**
 * Attempt to draw missions.
 * return 
 * the number of missions received on success
 * 0 if there are no other missions left to request (DMX) (a different command can be sent)
 * < 0 on fail
	 * -1 on Wrong Content
	 * -2 on Wrong Demand
	 * -3 on Rejected command (not your turn)
	 * -4 on Unknown command
 */
int TTRClient::drawMissions(void)
{
	String  workCmd, workStr;
	int i;
	workStr = "CDM";
	workCmd = sendACommand(workStr);

	if(workCmd == "")
		return resultCode;

	workStr = "D";
	workStr += playerNumber + 1;
	workStr += "M";

	if(workCmd.length() < 3) return resultCode;
	if(workCmd.data[2] == 'X') return 0;	// if no more missions return 0

	if(workStr != workCmd.substring(0,3)) return resultCode;
	
	i = workCmd.length(); i-= 3; i /= 6;
	return i;
}

/**
 * Tell server we are keeping all missions.
 * return 
 * 0 on success 
 * < 0 on fail
 * -1 on Wrong Content
 * -2 on Wrong Demand
 * -3 on Rejected command (not your turn)
 * -4 on Unknown command
 */
int TTRClient::keepAllMissions(void)
{
	String  workCmd, workStr;
	workStr = "CE";
	workCmd = sendACommand(workStr);
	if(workCmd == "")
		return resultCode;

	workStr = "E";
	workStr += playerNumber + 1;
	workStr += "0";

	if(workCmd.length() != 3)
		return resultCode;

	if(workStr != workCmd)
		return resultCode;

	return SUCCESS;
}

/**
 * Tell server we are rejecting mission1 and mission2 (may be null).
 * param mission1 first mission to reject.
 * param mission2 second mission to reject.
 * return 
	2 on two missions rejected
	1 on one mission rejected
	0 on 0 missions rejected
	< 0 on fail
	-1 on Wrong Content
	-2 on Wrong Demand
	-3 on Rejected command (not your turn)
	-4 on Unknown command
 */
int TTRClient::rejectMissions(TTRMission *mission1, TTRMission *mission2)
{
	if (myPlayer->firstRun){
		myPlayer->firstRun = false;
		if (mission1 != NULL) myPlayer->nInitialMissions--;
		if (mission2 != NULL) myPlayer->nInitialMissions--;
	}

	String  workCmd, workStr; int i,k;
	workStr = "CE"; k = 0;
	if(mission1 != (TTRMission *)0)
	{
		i = mission1->getNodeA()->getId();
		if(i<10) workStr += "0";
		workStr += i;

		i = mission1->getNodeB()->getId();
		if(i<10) workStr += "0";
		workStr += i;

		i = mission1->getValue();
		if(i<10) workStr += "0";
		workStr += i;

		k++;
	}

	if(mission2 != (TTRMission *)0)
	{
		i = mission2->getNodeA()->getId();
		if(i<10) workStr += "0";
		workStr += i;

		i = mission2->getNodeB()->getId();
		if(i<10) workStr += "0";
		workStr += i;

		i = mission2->getValue();
		if(i<10) workStr += "0";
		workStr += i;

		k++;
	}

	if (workStr == "CE") return keepAllMissions();
	workCmd = sendACommand(workStr);
	if(workCmd == "")
		return resultCode;

	workStr = "E";
	workStr += (playerNumber+1); workStr += k;

	if(workCmd.length() != 3) return resultCode;
	if(workStr != workCmd) return resultCode;

	myPlayer->removeMission(mission1);
    myPlayer->removeMission(mission2);
	return k;
}


/**
 * Attempt to claim a route.
 * param node1 source node.
 * param node2 destination node.
 * param cardColor card color to attempt claim.
 * param wagons number of cars to attempt claim.
 * param engines number of engines to attempt claim.
 * return 
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
int TTRClient::claimRoute(int s, int d, char color, int cars, int engs)
{
	String  workCmd, workStr;

	workStr = "CC";
	if (s < 10) workStr += "0"; workStr += s;
	if (d < 10) workStr += "0"; workStr += d;
	workStr += color;
	if (cars < 10)  workStr += "0";
	workStr += cars;
	if (engs < 10) workStr += "0";
    workStr += engs;

	workCmd = sendACommand(workStr);
	if(workCmd == "")
		return resultCode;

	String tmp (playerNumber + 1);
	workStr = String("C") + tmp + workStr.substring(1,10);
	if(workCmd == workStr)
	{
		//myMap->claimRouteDummy(myPlayer, s, d, color, cars, engs);
    	return 0;
	}

	if(workCmd.length() !=12 && workCmd.length() != 7)
		return resultCode;

	int i = 0;		
	if(workCmd.data[2] == 'D')  i = workCmd.substring(3,1).toInt();

	// i know i've successfully claimed a tunnel route - so wait for confirmation 
	// i know the server is going to send it
	if (i == 0) {
		workCmd = waitForResponse();

		if (workCmd != workStr) {
			return resultCode; 
		}
	}

	return i;
}


/**
 * Pass on a failed claim.
 * return 
 	0 on success
	< 0 on fail
	-1 on Wrong Content
	-2 on Wrong Demand
	-3 on Rejected command (not your turn)
	-4 on Unknown command
 */
int TTRClient::claimPass(void)
{
	String  workCmd, workStr;
	workStr = "CCP";
	workCmd = sendACommand(workStr);
	if(workCmd == "")
		return resultCode;
	
	// DiM on success!
	workStr = "C";
	workStr += playerNumber + 1;
	workStr += "P";

	if(workCmd.length()!=3) return resultCode;
	if(workStr != workCmd) return resultCode;
	return 0;
}


/**
 * Attempt to build a station.
 * param initialNode source node.
 * param connectionNode destination node.
 * param cardColor card color to attempt build.
 * param wagons number of wagons of card color to attempt build.
 * param engines number of engines to attempt build.
 * return
 	0 on success
	< 0 on fail
	-1 on Wrong Content
	-2 on Wrong Demand
	-3 on Rejected command (not your turn)
	-4 on Unknown command
 */
int TTRClient::buildStation(int s, int d, char color, int cars, int engs)
{
	String  workCmd, workStr;
	workStr = "CB";
	if(s < 10) workStr += "0";
	workStr += s;
	if(d < 10) workStr += "0";
	workStr += d;
	workStr += color;
	if (cars < 10) workStr += "0";
	workStr += cars;
	if (engs <10) workStr += "0";
	workStr += engs;
	workCmd = sendACommand(workStr);

	if(workCmd == "")
		return resultCode;

	String tmp(playerNumber+1);
	workStr = String("B") + tmp + workStr.substring(2,9);
	if(workCmd == workStr)
	{
		//myMap->claimRouteDummy(myPlayer,s,d,color,cars,engs);
		return 0;
	}
	return resultCode;		
}

bool TTRClient::isLastRound(void)
{
	return lastRound;
}

TTRClient::~TTRClient(void){}
