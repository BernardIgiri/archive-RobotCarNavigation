/*
 * depthFirstSearch.cpp
 *
 *  Created on: Apr 23, 2009
 *      Author: bernard
 */

#include "depthFirstSearch.hpp"
#include "calcSteerDirection.hpp"

namespace graph {

void depthFirstSearch::operator()(const directedGraph &g,unsigned char start,unsigned char goal,
		graphEdge::directionT initialDirection,pathT &p,
		unsigned char blockedEdgeFrom,unsigned char blockedEdgeTo) const {
	nodeVecT visited;
	search(g,start,goal,initialDirection,p,visited,blockedEdgeFrom,blockedEdgeTo);
	// shift turns up 1 to correct directions
	int i=p.size();
	while (--i) {
		p[i]=graph::path(p[i-1].turn(),p[i].distance(),p[i].destination());
	}
	graph::path &last=p[i];
	last=path(path::STRAIGHT,last.distance(),last.destination());
}
bool depthFirstSearch::search(const directedGraph &g,unsigned char start,unsigned char goal,graphEdge::directionT prior,
		pathT &path,nodeVecT &visited,unsigned char blockedEdgeFrom,unsigned char blockedEdgeTo) const {
	// if the start is the goal then return that the goal has been found
	if (start==goal) { return true; }
	// if the start has already been visited then return that the goal cannot be found on this path
	if (visited.contains(start)) { return false; }
	// mark this node as visited
	visited.push(start);
	// check each edge of this node for a possible match of the goal
	const graphNode &nd = g.getNode(start);
	for (unsigned char i=0;i<3;i++) {
		unsigned char id=nd.edge(i).Destination();
		// if this edge is not set then stop looking for edges
		if (id==0) { break; }
		// if this edge is blocked then do not search it
		if (id!=blockedEdgeTo||start!=blockedEdgeFrom) {
			// if this edge has a match then fill out the path array recursively
			graphEdge::directionT d=nd.edge(i).Direction();
			if (search(g,id,goal,d,path,visited,blockedEdgeFrom,blockedEdgeTo)) {
				path::turnT turn=calcSteerDirection(prior,d);
				graph::path p(turn,nd.edge(i).Length(),nd.edge(i).Destination());
				path.push(p);
				return true;
			}
		}
	}
	return false;
}

}
