
#include "TTRPlayer.h"
#include "TTRMap.h"

TTRPlayer::TTRPlayer(char *_name, char *_team){
	score = 0; cars = 45;
	name = _name;
	team = _team;
	nStations = 3;

	missions = new ArrayList<TTRMission>();
	stations = new ArrayList<TTRNode>();
	
	cards = new TTRCard * [nColors+1];
	for (int i=0; i<nColors; i++)
		cards[i] = new TTRCard(colors[i], 0);
	cards[nColors] = new TTRCard('*', 4);

	Client = NULL;
	InGame = false;
	Ready = false;
    MyTurn = false;
	
	edgeScore = missionScore = 
		longestRoute = stationScore = 0;
		
	longestPath = false;

	firstRun = true;
	nInitialMissions = 3;
}

void TTRPlayer::clearAll(void)
{
	firstRun = true;
	nInitialMissions = 3;

       score = 0; cars = 45;
       nStations = 3;

       delete missions;
       missions = new ArrayList<TTRMission>();

       delete stations;
       stations = new ArrayList<TTRNode>();

       delete [] cards;
       cards = new TTRCard * [nColors];

       for (int i=0; i<nColors; i++)
               cards[i] = new TTRCard(colors[i], 0);

       InGame = false;
       Ready = false;
       MyTurn = false;

       edgeScore = missionScore =
               longestRoute = stationScore = 0;

       currentGame = (TTRMap *)0;        
	   longestPath = false;
}

void TTRPlayer::setCurrentGame(TTRMap *game){
	game->nPlayers++;
	currentGame = game;
}

TTRMap *TTRPlayer::getCurrentGame(void){
	return currentGame;
}

int TTRPlayer::getCars(void){return cars;}
void TTRPlayer::decCars(int d){cars -= d;}

void TTRPlayer::decCards(char c, int n){
	printf("TTRPlayer::decCards(%c, %i)\n", c, n);
	for (int i=0; i<nColors; i++)
		if (cards[i]->getColor() == c){
			printf("\tbefore = %i\n", cards[i]->getCount());
			cards[i]->decCount(n);
			printf("\tafter = %i\n", cards[i]->getCount());

			if (cards[i]->getCount() < 0){
				cards[nColors]->decCount(
					cards[i]->getCount());
				cards[i]->setCount(0);
			}
			break;
		}
}

int TTRPlayer::getNStations(void){
	return nStations;
}

String TTRPlayer::getName(void){
	return name;
}

String TTRPlayer::getTeam(void){
	return team;
}

ArrayList<TTRMission> *TTRPlayer::getMissionArray(void){
	return missions;
}

TTRMission **TTRPlayer::getMissions(void){
	return missions->getObjects();
}

ArrayList<TTRNode> *TTRPlayer::getStationArray(void){
	return stations;
}

TTRNode **TTRPlayer::getStations(void){
	return stations->getObjects();
}


TTRCard **TTRPlayer::getCards(void){
	return cards;
}

TTRCard *TTRPlayer::getCard(char c) {
	TTRCard **hand = getCards();
	for (int i=0; i<nColors+1; i++)
		if (hand[i]->getColor() == c)
			return hand[i];

	return NULL;
}

void TTRPlayer::add(TTRMission *mission){
	missions->add(mission);
}

void TTRPlayer::add(TTRCard *card){
	char color = card->getColor();
	for (int i=0; i<nColors+1; i++)
		if (cards[i]->getColor()==color){
			cards[i]->incCount(card->getCount());
			break;
		}
}

void TTRPlayer::addCard(char c){
	for (int i=0; i<nColors+1; i++)
		if (cards[i]->getColor()==c){
			cards[i]->incCount(1);
			break;
		}
}

TTRPlayer::~TTRPlayer(void){
//	delete [nColors] cards;
	delete [] cards;
	delete missions;
	delete stations;
}

void TTRPlayer::decNStations(void){
	nStations--;
}

void TTRPlayer::add(TTRNode *station){
	stations->add(station);
}

void TTRPlayer::incEdgeScore(int c){
	edgeScore += c;
}
int TTRPlayer::getEdgeScore(void){
	return edgeScore;
}

int TTRPlayer::getMissionScore(void){
	return missionScore;
}
	
void TTRPlayer::setStationScore(void){
	stationScore = 4 * nStations;
}
int TTRPlayer::getStationScore(void){
	return stationScore;
}

void TTRPlayer::setLongestPath(int l){
	longestRoute = l;
}
int  TTRPlayer::getLongestPath(void){
	return longestRoute;
}
	
void TTRPlayer::setScore(int mod){
	score = missionScore + edgeScore + stationScore + mod; 
}
int TTRPlayer::getScore(void){ return score;}

void TTRPlayer::removeCard(char c, int n){
	for (int i=0; i<nColors; i++)
		if (cards[i]->getColor() == c)
			if (cards[i]->getCount() >= n)
				cards[i]->decCount(n);
			else {
				int tmp = n - cards[i]->getCount();
				cards[i]->decCount(n - tmp);
				cards[nColors]->decCount(tmp);
			}
}

void TTRPlayer::removeMission(TTRMission *m)
{
	if (m != (TTRMission *)0)
       for (int i=0; i<missions->size(); i++)
               if (missions->get(i)==m)
                       delete missions->remove(i);
}
