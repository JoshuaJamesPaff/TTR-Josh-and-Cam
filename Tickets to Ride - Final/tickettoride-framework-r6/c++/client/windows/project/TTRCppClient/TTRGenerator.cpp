
#include "TTRGenerator.h"

long double 
	TTRGenerator::a = 742938285., 
	TTRGenerator::m = 0x7fffffff;

long double 
	TTRGenerator::D = 2147483648., 
	TTRGenerator::S = 1073741823;

int TTRGenerator::rst = 1000;

TTRGenerator::TTRGenerator(void){reset();}

long TTRGenerator::get(void){
	if ((count++) > rst) reset();
	long double p = TTRGenerator::a * z;
	z = p - (unsigned long)
		(p / TTRGenerator::m) * TTRGenerator::m;
	return z;
}

double TTRGenerator::frn(void){
	TTRGenerator::get();
	return (double)z/TTRGenerator::D;
}

void TTRGenerator::reset(void){
	z = (long)abs((int)TTRTime::get()); count = 0;
}

TTRGenerator::~TTRGenerator(void){

}
