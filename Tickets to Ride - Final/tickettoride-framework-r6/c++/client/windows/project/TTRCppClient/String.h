
#pragma once

class String {

private:

protected:

	/**
	 * String data.
	 */
	char *mData;
	
	/**
	 * String length.
	 */
	int mLength;

	/**
	 * Method that measures a null-terminated string's length.
	 * @param s null-terminated string to be measured.
	 * @return length of the string without the null-terminator.
	 */
	int length(char *s);

	/**
	 * Copies string s to mData.
	 * @param s string to be copied.
	 */
	void copy(char *s);

	/**
	 * Integer to ASCII conversion function. Saves string in mData.
	 * @param i integer to be converted
	 * @param b base for i
	 */
	void itoa(int i, int b);
	
	/**
	 * ASCII to integer conversion function. Converts the string in mData to int.
	 * @param b number base.
	 * @return the number.
	 */
	int  atoi(int b);
	
	/**
	 * Long to ASCII conversion function. Saves string in mData.
	 * @param l long to be converted.
	 * @param b base for l.
	 */
	void ltoa(long l, long b);
	
	/**
	 * ASCII to long conversion function. Converts the string in mData to long.
	 * @param b number base.
	 * @return the number.
	 */
	long atol(long b);

	/**
	 * Concatenates string s to mData.
	 * @param s string to be added.
	 */
	void concat(char *s);
	
	/**
	 * Concatenates strings s1 and s2 in mData.
	 * @param s1 first string.
	 * @param s2 second string.
	 * @warning deletes previous mData.
	 */
	void concat(char *s1, char *s2);

public:

	/**
	 * String raw access pointer.
	 * @warning use wisely.
	 */
	char *data;

	/**
	 * Void constructor.
	 */
	String(void);
	
	/**
	 * Char consrtructor. Builds a string from a single character.
	 * @param c the character.
	 */
	String(char c);
	
	/**
	 * Char * constructor. Builds a string from a char array.
	 * @param s char array.
	 */
	String(char *s);
	
	/**
	 * Int constructor. Builds a string from an integer (uses itoa).
	 * @param i the integer.
	 */
	String(int i);
	
	/**
	 * Long constructor. Builds a string from a long int (uses ltoa).
	 * @param l the number.
	 */
	String(long l);
	
	/**
	 * Copy constructor. Copies string s.
	 * @param s the string.
	 */
	String(const String& s);

	/**
	 * Returns mLength (the length of the string).
	 */
	int length(void);

	/**
	 * Assignment operator.
	 * @param c builds a string from char c.
	 * @return returns this string object by value.
	 */
	String operator= (char c);
	
	/**
	 * Assignment operator.
	 * @param s builds a string from char array s.
	 * @return returns this string object by value.
	 */
	String operator= (char *s);
	
	/**
	 * Assignment operator.
	 * @param i builds a string from int i (uses itoa).
	 * @return returns this string object by value.
	 */
	String operator= (int i);
	
	/**
	 * Assignment operator.
	 * @param l builds a string from long int l (uses ltoa).
	 * @return returns this string object by value.
	 */
	String operator= (long l);
	
	/**
	 * Assignment operator.
	 * @param s copies string s into mData.
	 * @return returns this string object by value.
	 */
	String operator= (String s);

	/**
	 * Adition operator.
	 * @param c adds c to mData in a new string object.
	 * @return new string object equal to (mData + c).
	 */
	String operator+ (char c);
	
	/**
	 * Adition operator.
	 * @param s adds s to mData in a new string object.
	 * @return new string object equal to (mData + s).
	 */
	String operator+ (char *s);
	
	/**
	 * Adition operator.
	 * @param i adds i to mData in a new string object.
	 * @return new string object equal to (mData + itoa( i ) ).
	 */
	String operator+ (int i);
	
	/**
	 * Adition operator.
	 * @param l adds l to mData in a new string object.
	 * @return new string object equal to (mData + ltoa( l ) ).
	 */
	String operator+ (long l);
	
	/**
	 * Adition operator.
	 * @param s adds s.data to mData in a new string object.
	 * @return new string object equal to (mData + s.data).
	 */
	String operator+ (String s);

	/**
	 * Adition assignment operator.
	 * @param c concatenates char c to mData.
	 */
	String operator+= (char c);
	
	/**
	 * Adition assignment operator.
	 * @param s concatenates char array s to mData.
	 */
	String operator+= (char *s);
	
	/**
	 * Adition assignment operator.
	 * @param i concatenates int i to mData (uses itoa).
	 */
	String operator+= (int i);
	
	/**
	 * Adition assignment operator.
	 * @param l concatenates long int l to mData (uses ltoa).
	 */
	String operator+= (long l);
	
	/**
	 * Adition assignment operator.
	 * @param s concatenates string s to mData (uses itoa).
	 */
	String operator+= (String s);

	/**
	 * Equal to operator. 
	 * @param c char to compare to.
	 * @return true if this string is composed of a single character equal to c.
	 */
	bool operator== (char c);
	
	/**
	 * Equal to operator. 
	 * @param s char array to compare to.
	 * @return true if mData and s contain the same sequence of characters.
	 */
	bool operator== (char *s);
	
	/**
	 * Equal to operator. 
	 * @param i integer to compare to.
	 * @return true if mData and itoa( i ) contain the same sequence of characters.
	 */
	bool operator== (int i);
	
	/**
	 * Equal to operator. 
	 * @param l long integer to compare to.
	 * @return true if mData and ltoa( l ) contain the same sequence of characters.
	 */
	bool operator== (long l);
	
	/**
	 * Equal to operator. 
	 * @param s string to compare to.
	 * @return true if mData and s.data) contain the same sequence of characters.
	 */
	bool operator== (String s);

	/**
	 * Not equal to operator. 
	 * @param s char array to compare to.
	 * @return false if mData and s contain the same sequence of characters.
	 */
	bool operator!= (char c);
	
	/**
	 * Not equal to operator. 
	 * @param s char array to compare to.
	 * @return false if mData and s contain the same sequence of characters.
	 */
	bool operator!= (char *s);
	
	/**
	 * Equal to operator. 
	 * @param i integer to compare to.
	 * @return false if mData and itoa( i ) contain the same sequence of characters.
	 */
	bool operator!= (int i);
	
	/**
	 * Not equal to operator. 
	 * @param l long integer to compare to.
	 * @return false if mData and ltoa( l ) contain the same sequence of characters.
	 */
	bool operator!= (long l);
	
	/**
	 * Not equal to operator. 
	 * @param s string to compare to.
	 * @return false if mData and s.data) contain the same sequence of characters.
	 */
	bool operator!= (String s);

	/**
	 * Substring method.
	 * @param i substring start position.
	 * @param j desired substring length
	 * @return new string object that contains chracters from mData starting from i until i+j.
	 */
	String substring(int i, int j);

	/**
	 * Trim whitespaces from beginning and end of string.
	 * @return new string object containing the new string.
	 */
	String trim(void);

	/**
	 * Transform mData to integer (uses atoi).
	 * @return int value of string.
	 * @warning does not check for number values.
	 */
	int toInt(void);

	/**
	 * Check if mData contains the subsequence s.
	 * @param s subsequence to search for.
	 * @return positio where first encountered.
	 */
	int pos(char *s);

	/**
	 * Destructor.
	 */
	virtual ~String(void);
};
