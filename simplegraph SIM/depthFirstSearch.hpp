/*
 * depthFirstSearch.hpp
 *
 *  Created on: Apr 22, 2009
 *      Author: bernard
 */

#ifndef DEPTHFIRSTSEARCH_HPP_
#define DEPTHFIRSTSEARCH_HPP_

#include "array.hpp"
#include "graph.hpp"
#include "path.hpp"

namespace graph {

class depthFirstSearch {
public:
	// type used for an array of graph node ids
	typedef mystl::array<unsigned char,25> nodeVecT;
	// performs a depth first search of graph g for the goal id from the start id
	// and returns the path in the path variable
	depthFirstSearch(const directedGraph &g,unsigned char start,unsigned char goal,
			graphEdge::directionT initialDirection,pathT &path,
			unsigned char blockedEdgeFrom=0,unsigned char blockedEdgeTo=0) {
		operator()(g,start,goal,initialDirection,path,blockedEdgeFrom,
			blockedEdgeTo);
	}
	// performs a depth first search of graph g for the goal id from the start id
	// and returns the path in the path variable
	void operator()(const directedGraph &g,unsigned char start,unsigned char goal,
			graphEdge::directionT initialDirection,pathT &path,
			unsigned char blockedEdgeFrom=0,unsigned char blockedEdgeTo=0) const;
private:
	// continues the DFS recursively using the visited array to keep track of visited nodes
	bool search(const directedGraph &g,unsigned char start,unsigned char goal,graphEdge::directionT prior,
			pathT &path,nodeVecT &visited,
			unsigned char blockedEdgeFrom,unsigned char blockedEdgeTo) const;
};

}

#endif /* DEPTHFIRSTSEARCH_HPP_ */
