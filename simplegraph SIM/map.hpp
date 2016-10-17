/*
 * map.hpp
 *
 *  Created on: May 28, 2009
 *      Author: bernard
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include "graph.hpp"

using namespace graph;

// create a  graph with x nodes
directedGraph map;

directedGraph &getMap() {
	// node pointer used for convenience in modifying the graph
	graphNode *nd=NULL;

	// select each node in the graph by id starting from 1
	// then add the edges to each node in the format of
	// setEdge(edgeId, graphEdge( destinationNodeID, length, edgeDirection) )
	nd=&map.getNode(1);
	nd->setEdge(0,graphEdge(2, 3, graphEdge::EAST));

	nd=&map.getNode(2);
	nd->setEdge(0,graphEdge(6, 3, graphEdge::NORTH));
	nd->setEdge(1,graphEdge(3, 3, graphEdge::EAST));

	nd=&map.getNode(3);
	nd->setEdge(0,graphEdge(4, 2, graphEdge::EAST));

	nd=&map.getNode(4);
	nd->setEdge(0,graphEdge(8, 3, graphEdge::NORTH));

	nd=&map.getNode(5);
	nd->setEdge(0,graphEdge(1, 3, graphEdge::SOUTH));
	nd->setEdge(1,graphEdge(6, 3, graphEdge::EAST));

	nd=&map.getNode(6);
	nd->setEdge(0,graphEdge(10, 4, graphEdge::NORTH));

	nd=&map.getNode(7);
	nd->setEdge(0,graphEdge(6, 3, graphEdge::WEST));

	nd=&map.getNode(8);
	nd->setEdge(0,graphEdge(7, 2, graphEdge::WEST));
	nd->setEdge(1,graphEdge(12, 4, graphEdge::NORTH));

	nd=&map.getNode(9);
	nd->setEdge(0,graphEdge(13, 3, graphEdge::NORTH));
	nd->setEdge(1,graphEdge(5, 4, graphEdge::SOUTH));

	nd=&map.getNode(10);
	nd->setEdge(0,graphEdge(9, 3, graphEdge::WEST));
	nd->setEdge(1,graphEdge(11, 3, graphEdge::EAST));

	nd=&map.getNode(11);
	nd->setEdge(0,graphEdge(12, 2, graphEdge::EAST));
	nd->setEdge(1,graphEdge(7, 4, graphEdge::SOUTH));

	nd=&map.getNode(12);
	nd->setEdge(0,graphEdge(16, 3, graphEdge::NORTH));

	nd=&map.getNode(13);
	nd->setEdge(0,graphEdge(14, 3, graphEdge::EAST));

	nd=&map.getNode(14);
	nd->setEdge(0,graphEdge(10, 3, graphEdge::SOUTH));

	nd=&map.getNode(15);
	nd->setEdge(0,graphEdge(14, 3, graphEdge::WEST));

	nd=&map.getNode(16);
	nd->setEdge(0,graphEdge(15, 2, graphEdge::WEST));

	return map;
}

#endif /* MAP_HPP_ */
