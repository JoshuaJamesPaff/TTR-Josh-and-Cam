
#include "String.h"

int String::length(char *s){
	if (!s) return 0;
	else {
		int ret = 0;
		while (s[ret]!='\0') ret++;
		return ret;
	}
}

void String::copy(char *s){
	if (mData) delete [] mData;
	mLength = length(s);
	mData = new char [mLength+1];
	for (int i=0; i<mLength; i++) 
		mData[i] = s[i];
	mData[mLength] = '\0';
	data = mData;
}

void String::itoa(int v, int b){
	if (mData) delete mData;
	int tmp = v; mLength = 0;
	if (tmp == 0) mLength++;
	while (tmp > 0) {
		tmp /= b; mLength++;
	}
	mData = new char [mLength+1]; 
	tmp = v;
	for (int p=0; p<mLength; p++){
		mData[mLength-p-1] = (char)((tmp%b)+48);
		tmp /= b;
	}
	mData[mLength] = '\0';
	data = mData;
}

int  String::atoi(int b){
	if (mData){
		int ret = 0, base = 1;
		for (int p=mLength-1; p>=0; p--){
			ret += (int)(mData[p] - 48) * base;
			base *= b;
		}
		return ret;
	} else return 0;
}

void String::ltoa(long v, long b){
	if (mData) delete mData;
	long tmp = v; mLength = 0;
	while (tmp > 0) {
		tmp /= b; mLength++;
	}
	mData = new char [mLength+1]; 
	tmp = v;
	for (int p=0; p<mLength; p++){
		mData[mLength-p-1] = (char)((tmp%b)+48);
		tmp /= b;
	}
	mData[mLength] = '\0';
	data = mData;
}

void String::concat(char *s){
	char *tmp = mData;
	mData = (char *)0;
	concat(tmp, s);
	delete [] tmp;
}

void String::concat(char *s1, char *s2){
	if (mData) delete [] mData;
	int len1 = length(s1), len2 = length(s2);
	mData = new char [len1 + len2 + 1];
	for (int i=0; i<len1; i++) mData[i] = s1[i];
	for (int i=0; i<len2; i++) mData[i+len1] = s2[i];
	mData[len1 + len2] = '\0';
	mLength = len1 + len2;
	data = mData;
}

String::String(void){
	mLength = 0;
	mData = (char *)0;
	data = mData;
}

String::String(char c){
	char s[2] = {c, '\0'};
	mData = (char *)0; copy(s);
}

String::String(char *s){
	mData = (char *)0; copy(s);
}

String::String(int i){
	mData = (char *)0; itoa(i, 10);
}

String::String(long l){
	mData = (char *)0; ltoa(l, 10);
}

String::String(const String& s){
	mData = (char *)0; copy(s.data);
}

int String::length(void){
	return mLength;
}

String String::operator= (char c){
	char s[2] = {c, '\0'};
	copy(s); return (*this);
}

String String::operator= (char *s){
	copy(s); return (*this);
}

String String::operator= (int i){
	String s(i); copy(s.data); return (*this);
}

String String::operator= (long l){
	String s(l); copy(s.data); return (*this);
}

String String::operator= (String s){
	copy(s.data); return (*this);
}

String String::operator+ (char c){
	char s[2] = {c, '\0'};
	String ret(data); ret += s; return ret;
}

String String::operator+ (char *s){
	String ret(data); ret += s; return ret;
}

String String::operator+ (int i){
	String ret(data); ret += i; return ret;
}

String String::operator+ (long l){
	String ret(data); ret += l; return ret;
}

String String::operator+ (String s){
	String ret(data); ret += s; return ret;
}

String String::operator+= (char c){
	char s[2] = {c, '\0'};
	concat(s); return (*this);
}

String String::operator+= (char *s){
	concat(s); return (*this);
}

String String::operator+= (int i){
	String s(i); concat(s.data); return (*this);
}

String String::operator+= (long l){
	String s(l); concat(s.data); return (*this);
}

String String::operator+= (String s){
	concat(s.data); return (*this);
}

bool String::operator== (char c){
	if (mLength != 1) return false;
	if (data[0] != c) return false;
	return true;
}

bool String::operator== (char *s){
	int len = length(s); 
	if (mLength != len) return false;
	for (int i=0; i<len; i++)
		if (data[i] != s[i]) return false;
	return true;
}

bool String::operator== (int i){
	return (*this) == String(i);
}

bool String::operator== (long l){
	return (*this) == String(l);
}

bool String::operator== (String s){
	if (mLength != s.length()) return false;
	for (int i=0; i<mLength; i++)
		if (mData[i] != s.data[i]) return false;
	return true;
}

bool String::operator!= (char c){
	return (!(*this == c));
}

bool String::operator!= (char *s){
	return (!(*this == s));
}

bool String::operator!= (int i){
	return (!(*this == i));
}

bool String::operator!= (long l){
	return (!(*this == l));
}

bool String::operator!= (String s){
	return (!(*this == s));
}

String String::substring(int i, int j){
	if ((i<0) || (j<0) || (i+j > mLength))	return String("");
	char *tmp = new char [j+1];
	for (int k=0; k<j; k++) tmp[k] = mData[i+k];
	tmp[j] = '\0';
	return String(tmp);
}

String String::trim(void){
	if (mData){
		int lc = 0, rc = 0; 
		unsigned char ch; 
		for(int i=0; i<mLength; i++){
			ch = mData[i]; 
			if(ch <= ' ')  lc++; 
			else  break; 
		} 
		for(int i = mLength-1; i>=0; i--){
			ch = mData[i]; 
			if(ch<=' ')  rc++;
			else  break; 
		} 
		return this->substring(lc, mLength-lc-rc); 
	} else 
		return String("");
}

int String::toInt(void){
	return atoi(10);
}

int String::pos(char *s){
	int len = length(s);
	for (int i=0; i<mLength; i++){
		bool found = true;
		for (int j=0; j<len; j++)
			if (mData[i+j] != s[j]){
				found = false;
				break;
			}
		if (found) return i;
	}
	return -1;
}

String::~String(void){
	if (mData) delete [] mData;
}
