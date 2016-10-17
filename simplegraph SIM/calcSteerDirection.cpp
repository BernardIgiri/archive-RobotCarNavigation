#include "calcSteerDirection.hpp"

namespace graph {

path::turnT calcSteerDirection(graphEdge::directionT fromHeading,graphEdge::directionT toHeading) {
  path::turnT turn=path::STRAIGHT;

  if (fromHeading==graphEdge::NORTH) {
    if (toHeading==graphEdge::EAST) {
      turn=path::RIGHT;
    } else if(toHeading==graphEdge::WEST) {
      turn=path::LEFT;
    }
  }
  if (fromHeading==graphEdge::EAST) {
    if (toHeading==graphEdge::SOUTH) {
      turn=path::RIGHT;
    } else if(toHeading==graphEdge::NORTH) {
      turn=path::LEFT;
    }
  }
  if (fromHeading==graphEdge::SOUTH) {
    if (toHeading==graphEdge::WEST) {
      turn=path::RIGHT;
    } else if(toHeading==graphEdge::EAST) {
      turn=path::LEFT;
    }
  }
  if (fromHeading==graphEdge::WEST) {
    if (toHeading==graphEdge::NORTH) {
      turn=path::RIGHT;
    } else if(toHeading==graphEdge::SOUTH) {
      turn=path::LEFT;
    }
  }
  return turn;
}

}