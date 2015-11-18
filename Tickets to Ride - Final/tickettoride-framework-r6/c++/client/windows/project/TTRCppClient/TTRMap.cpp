
#include "TTRMap.h"

TTRMap::TTRMap(void)
{
	nPlayers	 = 0;
	nodeList 	 = (ArrayList<TTRNode> *)0;

	adj 		 = (TTREdge ***)0;
	openDeck 	 = (TTRDeck<TTRCard> *)0;
}

int TTRMap::loadFromFile(char *filename)
{
	printf("load map from file : %s\n", filename);
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		return 0;
	} else {	
		TiXmlNode *root = doc.RootElement();
		if (!root) return 0;
		TiXmlElement *elem = 0;

		TiXmlNode *nodes_node = root->FirstChild("nodes");
		if (!nodes_node)
		{
			delete root; return 0;
		}
		nodeList = new ArrayList<TTRNode>();
		for (elem = nodes_node->FirstChildElement("node"); 
				elem; elem = elem->NextSiblingElement())
		{
			TTRNode *new_node = new TTRNode((char *)elem->Attribute("name"));
			new_node->setId(nodeList->size());
			nodeList->add(new_node);
		}
		nAdj = nodeList->size();
		adj = new TTREdge ** [nAdj];
		for (int i=0; i<nAdj; i++)
		{
			adj[i] = new TTREdge * [nAdj];
			for (int j=0; j<nAdj; j++)
				adj[i][j] = (TTREdge *)0;
		}

		TiXmlNode *edges_node		= root->FirstChild("edges");
		if (!edges_node)
		{
			delete root; return 0;
		}
		for (elem = edges_node->FirstChildElement("edge"); 
				elem; elem = elem->NextSiblingElement())
		{
			String 	node1((char *)elem->Attribute("node1")), 
					node2((char *)elem->Attribute("node2"));
			int i = nodeList->getPosByName(node1), 
				j = nodeList->getPosByName(node2);
			if ((i!=-1) && (j!=-1))
			{
				char col1, col2; const char *tmp;
				tmp = elem->Attribute("color1"); col1 = tmp[0];
				// Add backward compatibility with old maps
				tmp = elem->Attribute("color2");
				col2 = tmp[0];
				if (!strcmp(tmp, "none"))
					col2 = '0';

				adj[i][j] = new TTREdge(nodeList->get(i), nodeList->get(j), 
					atoi(elem->Attribute("value")), col1, col2,
					atoi(elem->Attribute("eng")), atoi(elem->Attribute("cars")), 
												atoi(elem->Attribute("tunnel")));
				adj[i][j]->setId(i, j);
				adj[j][i] = adj[i][j];
			} else {
				// error message here
			}
		}
		openDeck = new TTRDeck<TTRCard>();
		return 1;
	}
}

TTREdge ***TTRMap::getAdj(void)
{
	return adj;
}

int TTRMap::getNAdj(void)
{
	return nAdj;
}

ArrayList<TTRNode> *TTRMap::getNodeList(void)
{
	return nodeList;
}

String TTRMap::getName(void)
{
	return name;
}

TTRNode *TTRMap::getNode(int n)
{
	if ((n<nodeList->size())&&(n>=0))
		return nodeList->get(n);
	else return (TTRNode *)0;
}

TTRDeck<TTRCard> *TTRMap::getOpenDeck(void)
{
	return openDeck;
}

void TTRMap::claimRouteDummy(TTRPlayer *p, int i, int j, char c, int cars, int engs)
{
	if (i < 0 || i >= nAdj || j<0 || j>=nAdj )
		return;

	if (adj[i][j] == (TTREdge *)0)
		return;

	// Find which route to mark
	int n = -1;
	if ((adj[i][j]->getColor(0) == c || adj[i][j]->getColor(0) == '*') &&
			adj[i][j]->getOwner(0) == NULL)
		n = 0;
	else if (adj[i][j]->nColors == 2)
		if ((adj[i][j]->getColor(1) == c || adj[i][j]->getColor(1) == '*') &&
				adj[i][j]->getOwner(1) == NULL)
			n = 1;
	// These checks should also include the possibility to even use double routes,
	// but that information is not available here

	if (n == -1)
		return;

	if (adj[i][j]->getEngines() > engs)
		return;

	if (adj[i][j]->getEngines() + adj[i][j]->getCars() > cars + engs)
		return;

	TTRCard *carCard = NULL, *engCard = NULL, *wildCard;
	
	TTRCard **cards = p->getCards();
	wildCard = cards[nColors];
	
	assert(cards[8]->getColor() == 'e');
	for (int k = 0; k < nColors; k++) 
	{
		TTRCard *card = cards[k];
		if (card->getColor()==c) 
		{
			carCard = card;
		}
		if (card->getColor()=='e')
		{
			engCard = card;
		}
	}
	assert(engCard != NULL);

	if (carCard) 
	{
		int dec, extra = 0;
		if (carCard->getCount() < cars)
		{
			dec = carCard->getCount();
			extra = cars - dec;
		} 
		else dec = cars;
		carCard->decCount(dec);
		wildCard->decCount(extra);
	}
	
	if (engCard) 
	{
		int dec, extra = 0;
		if (engCard->getCount() < engs)
		{
			dec = engCard->getCount();
			extra = engs - dec;
		} 
		else dec = engs;
		engCard->decCount(dec);
		wildCard->decCount(extra);
	}
	
	p->decCars(adj[i][j]->getCars() + adj[i][j]->getEngines());

	//p->decCards(c, cars); p->decCards('e', engs);

	adj[i][j]->setOwner(p, c);
}

bool TTRMap::Claim(TTRPlayer *p, int i, int j, char c, int cars, int engs)
{
	if ((i<0)||(i>=nAdj)||(j<0)||(j>=nAdj)) return false;
	if (adj[i][j]==(TTREdge *)0) return false;

	if (adj[i][j]->getEngines() > engs) return false;
	if (adj[i][j]->getEngines() + adj[i][j]->getCars() >
		cars + engs) return false;

	if ((adj[i][j]->getColor(0) != c)&&(adj[i][j]->getColor(1) != c))
		return false;
	if ((c==adj[i][j]->getColor(0) && (adj[i][j]->getOwner(0)!=(TTRPlayer *)0)))
		return false;
	if ((c==adj[i][j]->getColor(1) && (adj[i][j]->getOwner(1)!=(TTRPlayer *)0)))
		return false;

	TTRCard **cards = p->getCards();
	for (int k = 0; k < nColors; k++) 
	{
		TTRCard *card = cards[k];
		if (card->getColor()==c) 
		{
			if (card->getCount() < cars) return false;
		}
		else if (card->getColor()=='e')
			if (card->getCount() < engs) return false;
	}

	TTRCard *car_card = (TTRCard *)0,
          *eng_card = (TTRCard *)0;
	for (int k = 0; k < nColors; k++) 
	{
		TTRCard *card = cards[k];
		if (card->getColor()==c) 
		{
			car_card = card;
			if (card->getCount()<cars) return false;
		}
		if (card->getColor()=='e')
		{
			eng_card = card;
			if (card->getCount()<engs) return false;
		}
	}

	if (car_card) car_card->decCount(cars);
	if (eng_card) eng_card->decCount(engs);
	p->decCars(adj[i][j]->getCars() + adj[i][j]->getEngines());
	return true;
}

bool TTRMap::BuildStation(TTRPlayer *p, int s, int d, char c, int cars, int engs)
{
	if ((s<0)||(s>=nAdj)||(d<0)||(d>=nAdj)) return false;
	
	TTRCard **cards = p->getCards();
	TTRCard *car_card = (TTRCard *)0, *eng_card = (TTRCard *)0;
	for (int i = 0; i < nColors; i++) 
	{
		TTRCard *card = cards[i];
		if (card->getColor() == c)  car_card = card;
		if (card->getColor() == 'e')eng_card = card;
	}
	if ((!car_card)&&(!eng_card)) return false;
	if (car_card) if (car_card->getCount() < cars) return false;
	if (eng_card) if (eng_card->getCount() < engs) return false;

	if (adj[s][d] == (TTREdge *)0) return false;
	if (nodeList->get(s)->getPlayer() != (TTRPlayer *)0) return false;

	if (p->getNStations() < 1) return false;
	if (p->getStationArray()->size()+1 < cars + engs) return false;

	p->decNStations();
	p->add(nodeList->get(s));
	adj[s][d]->buildStation(p);
	nodeList->get(s)->setEdge(adj[s][d]);
	nodeList->get(s)->setPlayer(p);
	return true;
}

void TTRMap::removeCardFromOpenDeck(char c)
{
	for (int i=0; i<openDeck->size(); i++)
		if (openDeck->get(i)->getColor()==c)
			openDeck->remove(i);
}
	
void TTRMap::refreshOpenDeck(String  s)
{
	int n = openDeck->size();
	char *buf = s.data;
	for (int i=0; i<n; i++) 
		delete openDeck->remove(n-i-1);
	for (int i=0; i<s.length(); i++) 
		openDeck->add(new TTRCard(buf[i], 1));
}

int TTRMap::nCardsInDeck(void){
	// to do:
	return 0;
}

void TTRMap::cleanup(void)
{
	if (nodeList) delete nodeList;

	if (adj)
	{
		for (int i=0; i<nAdj; i++)
		{
			for (int j=0; j<nAdj; j++){
				if (adj[i][j]) delete adj[i][j];
				adj[i][j] = (TTREdge *)0;
			}
			delete [] adj[i];
		}
		delete [] adj;
	}

	if (openDeck) delete openDeck;
}

TTRMap::~TTRMap(void)
{
	cleanup();
}

void TTRMap::buildStationDummy(TTRPlayer *p, int s, int d, char c, int cars, int engs)
{
       if ((s<0)||(s>=nAdj)||(d<0)||(d>=nAdj)) return;
       
       TTRCard **cards = p->getCards();
       TTRCard *car_card = (TTRCard *)0, *eng_card = (TTRCard *)0;
       for (int i = 0; i < nColors; i++) {
               TTRCard *card = cards[i];
               if (card->getColor() == c)  car_card = card;
               if (card->getColor() == 'e')eng_card = card;
       }
       if ((!car_card)||(!eng_card)) return;
       TTRCard *wildCard = cards[nColors];

       if (adj[s][d] == (TTREdge *)0) return;
       if (nodeList->get(s)->getPlayer() != (TTRPlayer *)0) return;

       if (p->getNStations() < 1) return;
       if (p->getStationArray()->size()+1 < cars + engs) return;
       
       if (car_card)
	   {
               int dec = car_card->getCount(), extra = 0;
               if (car_card->getCount() < cars){
                       extra = cars - dec;
               } else dec = cars;
               car_card->decCount(dec); wildCard->decCount(extra);
       }
       
       if (eng_card)
	   {
               int dec = eng_card->getCount(), extra = 0;
               if (eng_card->getCount() < engs){
                       extra = engs - dec;
               } else dec = engs;
               eng_card->decCount(dec); wildCard->decCount(extra);
       }

       p->decNStations();
       p->add(nodeList->get(s));
       adj[s][d]->buildStation(p);
       nodeList->get(s)->setEdge(adj[s][d]);
       nodeList->get(s)->setPlayer(p);
 
}
