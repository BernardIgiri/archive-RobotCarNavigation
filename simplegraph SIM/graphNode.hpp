/*
 * graphNode.hpp
 *
 *  Created on: Apr 22, 2009
 *      Author: bernard
 */

#ifndef GRAPHNODE_HPP_
#define GRAPHNODE_HPP_

#include "graphEdge.hpp"

namespace graph {

class graphNode {
public:
	// default constructor
	graphNode() {};
	// copy constructor
	graphNode(const graphNode &g);
	// destructor
	~graphNode() {};
	// assignment operator
	graphNode &operator=(const graphNode &g);
	// sets the edge at index i to edge e
	void setEdge(unsigned char i,const graphEdge &e) { edges[i]=e; }
	// returns the edge at index i
	const graphEdge &edge(unsigned char i) const { return edges[i]; }
	// returns true if the matching edge is found and copies it to e
	bool findEdge(graphEdge &e,unsigned char destinationID) const;
private:
	// 6 byte array of edges
	graphEdge edges[3];
};

}

#endif /* GRAPHNODE_HPP_ */
