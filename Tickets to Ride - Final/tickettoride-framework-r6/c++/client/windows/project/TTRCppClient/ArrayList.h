#ifndef ArrayListH
#define ArrayListH

#include "String.h"

template <class Object>

/**
 * ArrayList object.
 */
class ArrayList {

private:

	/**
	 * Object array.
	 */
	Object **objects;

	/**
	 * Number of elements.
	 */
	int n;
	
	/**
	 * Maximum number of elements.
	 */
	int max;

	/**
	 * Reallocate memory when we run out of space for new objects.
	 */
	void realloc(void){	
		// realloc memory
		max <<= 1;
		Object **newVal = new Object* [max];
		for (int i=0; i<n; i++)
			newVal[i] = objects[i];
		Object **tmp = objects;
		objects = newVal;
	}

public:

	/**
	 * Void constructor. Initial size = 4;
	 */
	ArrayList(void){
		n = 0; max = 4;
		objects = new Object * [max];
	}

	/**
	 * Add object o to array.
	 * @param o object to add.
	 */
	void add(Object *o){
		// add Object o on position n;
		if (o){
			if (n >= max) realloc();
			objects[n] = o; n++;
		}
	}

	/**
	 * Add object o to position p. All objects after p are shifted a position.
	 * @param o object to add.
	 * @param p position to add o to.
	 */
	void add(int p, Object *o){	
		// add Object o on position p;
		if (o){
			if ((p<n+1)&&(p>=0)) {
				if (n >= max) realloc();
				for (int i=n-1; i>p; i--)
					objects[i+1] = objects[i];
				objects[p] = o; n++;
			}
		}
	}

	/**
	 * Replace object on position p with object o.
	 * @param p position of object to replace.
	 * @param o object to add to position p.
	 * @return object previously stored on position p.
	 */
	Object *replace(int p, Object *o){	
		// replace Object on position p with o
		if (o){
			Object *ret = (Object *)0;
			if ((p<n)&&(p>=0)) {
				ret = objects[p];
				objects[p] = o;
			}
			return ret;
		}
	}

	/**
	 * Size method.
	 * @return number of elements in array.
	 */
	int size(void) {return n;}

	/**
	 * Remove object o from array. All elements after o are shifted a position.
	 * @param o object to remove.
	 * @return removed object (o).
	 */
	Object *remove(Object *o){
		// remove Object o
		if (o){
			Object *ret = (Object *)0;
			for (int i=0; i<n; i++){
				if (objects[i]->equals(o)){
					for (int j=i; j<n-1; j++)
						objects[j] = objects[j+1];
					n--;
					objects[n] = (Object *)0;
					break;
				}
			}
			return ret;
		}
	}

	/**
	 * Remove object at position p.
	 * @param p position.
	 * @return removed object.
	 */
	Object *remove(int p){
		// remove Object on position p
		Object *ret = (Object *)0;
		if ((p>=0) && (p<n)){
			ret = objects[p];
			for (int i=p; i<n-1; i++)
				objects[i] = objects[i+1];
			n--;
			objects[n] = (Object *)0;
		}
		return ret;
	}

	/**
	 * Remove and delete object o from array. All objects after o are shifted a position.
	 * @param o object to delete.
	 */
	void del(Object *o){
		// delete Object o
		if (o) delete remove(o);
	}

	/**
	 * Remove and delete object at position p from array. All objects after o are shifted a position.
	 * @param p positionof object to remove.
	 */
	void del (int p){
		// delete Object at position p
		Object *o = remove(p);
		if (o) delete o;
	}

	/**
	 * Get object at position p.
	 * @param p object position.
	 */
	Object *get(int p){
		return objects[p];
	}

	/**
	 * Get raw object array access.
	 * @warning use wisely.
	 * @return raw object array pointer.
	 */
	Object **getObjects(void){
		return objects;
	}

	/**
	 * Get object position by name.
	 * @param object name.
	 * @return object position.
	 * @warning Object class needs to have a method getName() that returns a String object.
	 */
	int getPosByName(String name){
		int ret = -1;
		for (int i=0; i<n; i++){
			if (objects[i]->getName() == name){
				ret = i; break;
			}
		}
		return ret;
	}

	/**
	 * Destructor.
	 * @warning destroy remaining elements.
	 */
	~ArrayList(void){
		delete [n] objects;
	}
};
#endif
