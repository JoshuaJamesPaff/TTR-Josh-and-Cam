#include "RandomSolution.h"

RandomSolution::RandomSolution(char *user, char *pass, 
			char *agent, char *host, int port) 
	: TTRClient(user, pass, agent, host, port)
{
	myRand = new TTRGenerator();
	drawCounter = 0;
}
 
void RandomSolution::myMethod(void)
{
	// Set action performed flag to false as we have not acted this turn.
	actionPerformed = false;
		
	// If (firstMissions==true) reject last received mission.
	if (firstMissions) discardMissions();
	else 	
		// As long as we have not performed an action, attempt to choose an action to perform.
		while ((!actionPerformed)&&(play)&&(myTurn))
		{
			int action = 1;

			if(!countColors() || drawCounter < 26 ){
				action = 1; // if there are no 4ofakind in hand then draw cards
				
			}
			else{

				countColors();
				action = 3; // if there is then claim a route

			}
			
			// Let's see what we chose:
			switch (action) 
			{
			// Attempt to draw a card from the open deck.
				case DRAW_FROM_OPEN_DECK: attemptToDrawFromDeck(); break;
				
				// Attempt to draw a card from the blind deck.
				case DRAW_FROM_DECK: attemptToDrawFromDeck(); break;
				
				// Attempt to draw missions.
				case DRAW_MISSIONS: attemptToDrawMissions(); break;
				
				// Attempt to claim a route.
				case CLAIM_ROUTE: attemptToClaimRoute(); break;
				
				// Attempt to build a station.
				case BUILD_STATION: attemptToBuildStation(); break;
				
				// We have no idea what we tried to do; better try again. 
				default: break;
			}
		}
	}
//count colors in hand
bool RandomSolution::countColors(){

	int threshhold = 4;
	bool placeRoad = false;
	
	TTRCard **myCards = myPlayer->getCards();
	for (int i=0; i<=nColors; i++)
	{
		if (myCards[i]->getCount() >= threshhold)
		{
			sixColor = myCards[i]->getColor();
			placeRoad = true;
			break;
		}
	}
	return placeRoad;
}

void RandomSolution::discardMissions(){
	
	ArrayList<TTRMission> *m = myPlayer->getMissionArray();
	int ret = 0;
	int pos1;
	int pos2;
	int max = -1;
	//loop thru get first highest assign position to pos1
	for(int i = 0; i < m->size(); i++){
		if(m->get(i)->getValue() >  max){
			max = m->get(i)->getValue();
			pos1 = i;
		}
	}
	

	//loop thru and get second highest assign to pos2
	max = -1;
	for(int i = 0; i < m->size(); i++){

		//checks to see if same as first highest
		if(i==pos1)
		continue;
		if(m->get(i)->getValue() >  max){
			max = m->get(i)->getValue();
			pos2 = i;
		}
	}


		ret = rejectMissions(m->get(pos1), m->get(pos2));
		actionPerformed = true;
	
}



void RandomSolution::discardLastMission(){
	int ret = 0;
	ArrayList<TTRMission> *m = myPlayer->getMissionArray();
	while ((ret != 1)&&(play)){
		ret = rejectMissions(m->get(m->size()-1), (TTRMission *)0);
		actionPerformed = true;
	}
}

void RandomSolution::attemptToDrawFromDeck(){
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
	drawCounter++;
}
	
void RandomSolution::attemptToDrawMissions(){
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
	
void RandomSolution::attemptToClaimRoute(){
	// Initialize edge to claim.
	TTREdge *edge = (TTREdge *)0; 
	// Get number of nodes on map.
	int n = myMap->getNAdj(); 
	// Initialize source and destination node ids.
	int s=0, d=0;
	// As long as the edge we chose is null
	while (edge == (TTREdge *)0 )
	{
		bool sameColor = false;

		s = myRand->get() % n;
		d = myRand->get() % n;
		edge = myMap->getAdj()[s][d];


		// searches through routes to find one with the same color
		if(edge != (TTREdge *)0)
		{
			if(edge->getColor(0) == sixColor)
			{

			}else{
				s = myRand->get() % n;
				d = myRand->get() % n;
				edge = myMap->getAdj()[s][d];
			}

		}
	}



	// sets edge color.
	char color = edge->getColor(0);
	// Get out player's cards in hand.
	//TTRCard **myCards = myPlayer->getCards();
	// Get number of cars and engines required to claim route.
	int cars = edge->getCars(), engs = edge->getEngines();
	// Search for cards that match the edge's color.
	/*
	for (int i=0; i<nColors-1; i++)
	{
		if ((myCards[i]->getColor() == color) || (color == '*'))
		{
			TTRCard *card = myCards[i];
			if (card->getCount() < cars)
			{
				// Figure out how to pay for edge.
				engs += cars - card->getCount();
				cars = card->getCount();
			}
			color = card->getColor();
			break;
		}
	}
	*/
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
}
	
void RandomSolution::attemptToBuildStation(){
	// If we have no stations left, move on
	if (myPlayer->getNStations() == 0) return;
	// Initialize edge to build station on.
	TTREdge *edge = (TTREdge *)0; 
	// Get number of nodes on map.
	int n = myMap->getNAdj();
	// Initialize source and destination nodes.
	int s=0, d=0;
	// Randomly pick an edge.
	while (edge == (TTREdge *)0)
	{
		s = myRand->get()%n; 
		d = myRand->get()%n;
		edge = myMap->getAdj()[s][d];
	}
	// Get number of required cards to build station.
	int req = 4 - myPlayer->getNStations();
	char color = '0'; 
	// Get first card in card array to have enough cards to build station.
	TTRCard **myCards = myPlayer->getCards();
	for (int i=0; i<nColors-1; i++)
	{
		if (myCards[i]->getCount() >= req)
		{
			color = myCards[i]->getColor();
			break;
		}
	}
	// Attempt to build station.
	int ret = buildStation(s, d, color, req, 0);
	// If successful, set actionPerformed flag to true.
	if (ret==1) actionPerformed = true;
}

RandomSolution::~RandomSolution(void){}
