/*
 * graphNode.cpp
 *
 *  Created on: Apr 22, 2009
 *      Author: bernard
 */

#include "graphNode.hpp"

namespace graph {

graphNode::graphNode(const graphNode & g) {
	edges[0]=g.edges[0];
	edges[1]=g.edges[1];
	edges[2]=g.edges[2];
}

graphNode &graphNode::operator =(const graphNode &g) {
	edges[0]=g.edges[0];
	edges[1]=g.edges[1];
	edges[2]=g.edges[2];
	return *this;
}

bool graphNode::findEdge(graphEdge &e,unsigned char destinationID) const {
	for (int i=0;i<3;i++) {
		if (edges[i].Destination()==destinationID) {
		      e=edges[i];
		      return true;
		}
	}
	return false;
}

}
