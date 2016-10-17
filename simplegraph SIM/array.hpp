/*
 * array.hpp
 *
 *  Created on: Apr 11, 2009
 *      Author: bernard
 */

#ifndef ARRAY_HPP_
#define ARRAY_HPP_

#include <stdlib.h>

namespace mystl {

template <class T,int SIZE>
class array {
public:
	array() : nElmts(0) {};
	// add an element to the array
	inline bool push(const T &data) { return push_back(data); }
	// add an element to the front of the array & returns false on failure
	inline bool push_front(const T &data) {
		return insert(0,data);
	}
	// add an element to the back of the array & returns false on failure
	inline bool push_back(const T &data) { return insert(nElmts,data); }
	// remove the element at index idx from the array
	// this must be a valid index
	void remove(size_t idx) {
		if (nElmts>0) {
			--nElmts;
			for (;idx<nElmts;idx++) {
				ptr[idx]=ptr[idx+1];
			}
		}
	}
	// inserts data into the array at index pos & returns false on failure
	bool insert(size_t pos,const T &data) {
		if (pos<SIZE&&nElmts<SIZE) {
			size_t i = ++nElmts;
			while (i-- > pos) {
				ptr[i+1]=ptr[i];
			}
			ptr[pos]=data;
			return true;
		}
		return false;
	}
	// retrieves the element at array index i
	inline T &operator[](size_t i) {
		return ptr[i];
	}
	// retrieves a const reference to the element at array index i
	inline const T &operator[](size_t i) const {
		return ptr[i];
	}
	// removes all elements from the array
	inline void clear() {
		nElmts=0;
	}
	// returns true if the array is empty
	inline bool empty() const { return nElmts==0; }
	// returns the size of the array
	inline size_t size() const { return nElmts; }
	// returns the amount of memory allocated for the array
	inline size_t space() const { return SIZE; }
	// removes the first element from the array
	inline void remove_first() { remove(0); }
	// removes the last element from the array
	inline void remove_last() { if (nElmts>0) { remove(nElmts-1); } }
	// returns the first element in the array
	inline T &top() { return ptr[0]; }
	// returns a const reference to the first element in the array
	inline const T &top() const { return ptr[0]; }
	// returns the last element in the array
	inline T &bottom() { return ptr[nElmts-1]; }
	// returns a const reference to the last element in the array
	inline const T &bottom() const { return ptr[nElmts-1]; }
	// returns true if the array contains an element that is equal to data
	bool contains(const T &data) {
		size_t i=nElmts;
		while (i--) {
			if (ptr[i]==data) { return true; }
		}
		return false;
	}
private:
	// pointer to array memory
	T ptr[SIZE];
	// number of elements in array
	size_t nElmts;
};

}

#endif /* ARRAY_HPP_ */
