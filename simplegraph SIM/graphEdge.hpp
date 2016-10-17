/*
 * graphEdge.hpp
 *
 *  Created on: Apr 22, 2009
 *      Author: bernard
 */

#ifndef GRAPHEDGE_HPP_
#define GRAPHEDGE_HPP_

namespace graph {

class graphEdge {
public:
	// direction enumeration for edges
	enum directionT {
		NORTH = 0x00,
		SOUTH = 0x40,
		EAST = 0x80,
		WEST = 0xC0
	};
	// length mask uses the lower 6 bits to encode the length value
	static const unsigned char LENGTHMASK=0x3F;
	// direction shift value uses the upper 2 bits to encode the direction
	static const unsigned char DIRECTIONMASK=0xC0;
	// default constructor
	graphEdge() : length(0),destination(0) {}
	// copy constructor
	graphEdge(const graphEdge &e) : length(e.length),destination(e.Destination()) {}
	// creates a graph edge with the specified destination, length, and direction
	graphEdge(unsigned char Destination,unsigned char Length,directionT direction) : length(Length|direction),destination(Destination) {};
	// destructor
	~graphEdge() {};
	// assignment operator
	inline graphEdge &operator=(const graphEdge &e) { length=e.length;destination=e.Destination(); return *this; }
	// returns the length of the edge
	unsigned char Length() const { return length & LENGTHMASK; }
	// returns the direction the edge points in
	directionT Direction() const { return directionT(length & DIRECTIONMASK); }
	// returns the destination node id that the edge points to
	unsigned char Destination() const { return destination; }
	// returns true if this edge has been set
	bool valid() { return destination>0; }
private:
	// This object only occupies 2 bytes of RAM
	// max length = 63
	unsigned char length; // the first 6 bits are the length, the last 2 are the direction
	// max destination = 255
	unsigned char destination; // this is the id of the graph node in which this edge points to
};

}

#endif /* GRAPHEDGE_HPP_ */
