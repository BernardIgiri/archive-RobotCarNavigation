/*
 * graph.hpp
 *
 *  Created on: Apr 22, 2009
 *      Author: bernard
 */

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include "array.hpp"
#include "graphNode.hpp"
#include "graphEdge.hpp"

namespace graph {

class directedGraph {
public:
	// adds a node to the graph and returns its node id
	inline unsigned char addNode() { nodes.push(graphNode());return nodes.size(); }
	// returns the graph node with the specified id
	inline graphNode &getNode(unsigned char id) { return nodes[id-1]; }
	// returns a const reference to the graph node with the specified id
	inline const graphNode &getNode(unsigned char id) const { return nodes[id-1]; }
private:
	mystl::array<graphNode,25> nodes;
};

}

#endif /* GRAPH_HPP_ */
