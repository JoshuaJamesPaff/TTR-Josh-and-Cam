
#include "TTRCard.h"

TTRCard::TTRCard(char _color){
	color = _color; count = 0;
}

TTRCard::TTRCard(char _color, int _count){
	color = _color; count = _count;
}

char TTRCard::getColor(void){
	return color;
}

int TTRCard::incCount(int c){
	return count += c;	
}

int TTRCard::getCount(void){
	return count;
}

void TTRCard::setCount(int n){
	count = n;
}

int TTRCard::decCount(int c){
	return count -= c;
}

TTRCard::~TTRCard(void){

}