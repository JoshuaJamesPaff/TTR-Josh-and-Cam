
#include "TTRMission.h"

TTRMission::TTRMission(TTRNode *a, TTRNode *b, int val){
	nodeA = a; nodeB = b; value = val;
}

TTRNode *TTRMission::getNodeA(void){
	return nodeA;
}

TTRNode *TTRMission::getNodeB(void){
	return nodeB;
}

int TTRMission::getValue(void){
	return value;
}

TTRMission::~TTRMission(void){

}