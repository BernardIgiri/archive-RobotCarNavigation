#include "path.hpp"
#include "graphEdge.hpp"

namespace graph {
extern path::turnT calcSteerDirection(graphEdge::directionT fromHeading,
  graphEdge::directionT toHeading);
}