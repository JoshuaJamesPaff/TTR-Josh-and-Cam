
#include "TTREdge.h"

TTREdge::TTREdge(TTRNode *a, TTRNode *b, 
	int _val, char col0, char col1, 
		int eng, int _cars, 
			int _tunnel){
	nodeA = a; 
	nodeB = b;
	val = _val; 
	col[0] = col0; 
	col[1] = col1;
	if (col1 == '0') nColors = 1;
	else nColors = 2;
	engs = eng;
	cars = _cars;
	tunnel = _tunnel;
	owner[0] = owner[1] = 
		(TTRPlayer *)0;
	station[0] = (TTRPlayer *)0;
	station[1] = (TTRPlayer *)0;
}

void TTREdge::setId(int i, int j){
	id = i*radix + j;
}

int getId(void);

TTRNode *TTREdge::getNodeA(void){
	return nodeA;
}

TTRNode *TTREdge::getNodeB(void){
	return nodeB;
}

int TTREdge::getValue(void){
	return val;
}

int TTREdge::getEngines(void){
	return engs;
}

int TTREdge::getCars(void){
	return cars;
}

int TTREdge::getTunnel(void){
	return tunnel;
}

char TTREdge::getColor(int i){
	return col[i];
}

void TTREdge::setOwner(TTRPlayer *p, char c){
	/*
	if ((c == col[0] || col[0] == '*') && owner[0] == NULL)
		owner[0] = p;
	else if (nColors == 2)
		if ((c == col[1] || col[1] == '*') && owner[1] == NULL)
			owner[1] = p;
	*/
	if (((c==col[0]) || (col[0]=='*')) && (owner[0]==NULL))
		owner[0] = p;
	else if ((nColors > 1)&&((c==col[1])||(col[1]=='*')) && (owner[1]==NULL))
		owner[1] = p;
}

TTRPlayer *TTREdge::getOwner(int p){
	if ((p<2)&&(p>=0)) return owner[p];
	else return (TTRPlayer *)0;
}

void TTREdge::buildStation(TTRPlayer *p){
	if (station[0]==(TTRPlayer *)0) station[0] = p;
	else station[1] = p;
}

TTRPlayer *TTREdge::getStation(int i){
	if ((i>=0)&&(i<2)) return station[i];
	else return (TTRPlayer *)0;
}

int TTREdge::getId(void){
	return id;
}

TTREdge::~TTREdge(void){

}
