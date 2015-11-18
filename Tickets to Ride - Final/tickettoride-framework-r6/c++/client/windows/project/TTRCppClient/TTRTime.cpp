
#include "TTRTime.h"

unsigned __int64 TTRTime::get(void){
	unsigned __int64 tmpres = 0;
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
 
	tmpres |= ft.dwHighDateTime;
	tmpres <<= 32;
	tmpres |= ft.dwLowDateTime;

	tmpres /= 10;	// microsecs

	return tmpres;
}

unsigned __int64 TTRTime::delta(void){
	static unsigned __int64 oldtime = TTRTime::get();
	unsigned __int64 newtime = TTRTime::get();
	unsigned __int64 ret = newtime - oldtime;
	oldtime = newtime;
	return ret;
}