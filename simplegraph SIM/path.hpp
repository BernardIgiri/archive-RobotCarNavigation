/*
 * depthFirstSearch.hpp
 *
 *  Created on: Apr 22, 2009
 *      Author: bernard
 */

#ifndef PATH_HPP_
#define PATH_HPP_

#include "array.hpp"
#include "graph.hpp"

namespace graph {

class path {
public:
	enum turnT { STRAIGHT,LEFT,RIGHT};
	static const unsigned char DISTANCESHIFT=2;
	static const unsigned char TURNMASK=0x03;
	path() : value(STRAIGHT),destID(0) {};
	path(const path &p) : value(p.value),destID(p.destID)  {};
	path &operator=(const path &p) { value=p.value;destID=p.destID; return *this; }
	path(turnT pathTurn,unsigned char pathDistance,unsigned char destinationID) :
		value(pathTurn|(pathDistance<<DISTANCESHIFT)),destID(destinationID) {};
	turnT turn() const { return turnT(value&TURNMASK); }
	unsigned char distance() const { return value>>DISTANCESHIFT; }
	unsigned char destination() const { return destID; }
private:
	unsigned char value;
	unsigned char destID;
};

typedef mystl::array<path,25> pathT;

}

#endif /* PATH_HPP_ */
