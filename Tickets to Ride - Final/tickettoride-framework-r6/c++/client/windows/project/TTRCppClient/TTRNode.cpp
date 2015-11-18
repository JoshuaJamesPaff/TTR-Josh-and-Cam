
#include "TTRNode.h"

TTRNode::TTRNode(char *_name){
	name = _name;
	p = (TTRPlayer *)0; e = (TTREdge *)0;
	id = -1;
}

void TTRNode::setId(int _id){
	id = _id;
}

int TTRNode::getId(void){
	return id;
}

void TTRNode::setPlayer(TTRPlayer *_p){
	p = _p;
}

TTRPlayer *TTRNode::getPlayer(void){
	return p;
}

void TTRNode::setEdge(TTREdge *_e){
	e = _e;
}

String TTRNode::getName(void){
	return name;
}

TTRNode::~TTRNode(void){

}