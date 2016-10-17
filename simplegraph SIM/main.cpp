	/*
 * main.cpp
 *
 *  Created on: Apr 22, 2009
 *      Author: bernard
 */

#include "array.hpp"
#include "graph.hpp"
#include "depthFirstSearch.hpp"
#include "map.hpp"
#include "calcSteerDirection.hpp"

//#define SIMULATION 1

extern "C" {
#ifdef SIMULATION
	#include "sim.h"
#else
	#include "getIO.h"
	#include "run.h"
#endif
}

using namespace graph;

// Navigation and control defines
#define DISTANCE_TO_STOP				3500
#define DISTANCE_CONVERSION_FACTOR			0.03f
#define SPEED						1
#define WAIT_TIME						3
#define RE_ROUTE_TIMEOUT				1000
#define GRID_SCALE					90
#define END_CORRECTION					49
#define TURN_LEFT_DISTANCE				198
#define TURN_RIGHT_DISTANCE				202
#define TURNING_RADIUS					96
#define REVERSE_DISTANCE				150

// NAVIGATION STATES
enum state_t {
  DONE, // done driving end program
  WAITING, // waiting for obstical to move
  DRIVING, // driving to next waypoint
  STEERING, // steering
  REROUTE, // re routing to seek next target
  REVERSING // reversing if the return path requires it
};
enum nav_state_t {
  SEEKING_TARGET, // seeking first target
  RETURNING // seeking way home
};

// returns the distance to the next path target, for turns one grid unit
// is subtract for steering
inline int getDistanceTo(const path &p) {
  if (p.turn()==path::STRAIGHT) {
    return p.distance()*GRID_SCALE;
  }
	    #ifdef SIMULATION
	    printf("\nSubtract Radius\n");
	    #endif
  return p.distance()*GRID_SCALE-TURNING_RADIUS;
}

// calculates the direction between two nodes on graph
graphEdge::directionT getDirection(const directedGraph &map,unsigned char fromId,
  unsigned char toId) {
  graphEdge destEdge;
  graphNode destNode=map.getNode(fromId);
  if (destNode.findEdge(destEdge,toId)) {
    graphEdge::directionT dir= destEdge.Direction();
    #ifdef SIMULATION
    reportDirection(dir);
    #endif
    return dir;
  }
  // direction not found
  return graphEdge::NORTH;
}

// returns true if the goal has been reached
inline bool advancePathCursor(unsigned char &pathCursor,int &distanceToTarget,
  const graph::pathT &path,state_t &state,nav_state_t &nav,
  unsigned char &start,unsigned char &goal) {
  if (pathCursor==0) {
    if (nav==SEEKING_TARGET) {
      // target found, reroute to path home
      state=REROUTE;
      nav=RETURNING;
      // swap goal with start point to find path home
      unsigned char t=goal;
      goal=start;
      start=t;
      // pause at goal for at least 2 seconds
	pwm_set(PWM_STOP);
      delay_ms(2000);
    } else {
      state=DONE;
    }
    return true;
  } else {
    --pathCursor;
    distanceToTarget+=getDistanceTo(path[pathCursor]);
    if (pathCursor==0) { distanceToTarget-=END_CORRECTION; }
    if (path[pathCursor+1].turn()!=path::STRAIGHT) {
	    #ifdef SIMULATION
	    printf("\nSubtract Radius\n");
	    #endif
      distanceToTarget-=TURNING_RADIUS;
    }
  }
  return false;
}

int main() {
	// CREATE MAP
	directedGraph &map=getMap();
	// NAVIGATE ON MAP
	state_t state=DRIVING,priorState=DRIVING;
	nav_state_t nav=SEEKING_TARGET;
	
	path::turnT immediateTurn=path::STRAIGHT;
	
	unsigned char start=4,goal=13;
	unsigned char blockedFrom=0,blockedTo=0;
	graphEdge::directionT startDirection=graphEdge::NORTH;

	init_run();

	Command command;
	resetCommand(command);
	while (DRIVE_TO!=command.instruction||command.value<1) {
		getCommand(command);
	}
	goal=command.value;
	
	// vector for calculated path
	graph::pathT path;
	// run the depthFirstSearch algorithm on the graph
	depthFirstSearch(map,start,goal,startDirection,path);
	#ifdef SIMULATION
	printPath(path);
	#endif
	
	unsigned int lastTime=TCNT1;
	unsigned char pathCursor=path.size()-1;
	int distanceToTarget=getDistanceTo(path[pathCursor]);
	
	unsigned int waitTimer=0,rerouteTimer=0;

	pwm_set(PWM_GO);
	// DRIVE
	while (DONE!=state) {
		delay_ms(1);
		// get elapsed time
		unsigned int elapsed = TCNT1 - lastTime;
		lastTime=TCNT1;
		elapsed=1;
		// check for obsticals
		uint16_t obsticalDistance=ultra_sonic();
		
		if (obsticalDistance<DISTANCE_TO_STOP) {
			if (state!=WAITING) {
				distanceToTarget-=SPEED*elapsed;
				// preserve old state prior to entering wait state
				priorState=state;
			}
			pwm_set(PWM_STOP);
			state=WAITING;
		} else {
			// drive or wait
			switch (state) {
			case WAITING:
				pwm_set(PWM_STOP);
				waitTimer+=elapsed;
				rerouteTimer+=elapsed;
				if (rerouteTimer>RE_ROUTE_TIMEOUT) {
				  rerouteTimer=0;
				  // if the target is closer than the obstical
				  // then re-route around it
				  if (obsticalDistance*DISTANCE_CONVERSION_FACTOR>distanceToTarget) {
				    blockedFrom=start;
				    blockedTo=path[pathCursor].destination();
				    if ((int)path.size()<(int)(pathCursor+1)) {
				      blockedFrom=path[pathCursor+1].destination();
				    }
				    state=REROUTE;
				  }
				  // else wise there is nothing we can do
				} else if (waitTimer>WAIT_TIME) {
					waitTimer=0;
					// restore old state
					state=priorState;
					if (priorState==REVERSING) {
					  pwm_set(PWM_REVERSE);
					} else {
					  pwm_set(PWM_GO);
					}
				}
				break;
			case DRIVING:
				// track distance to target
				pwm_set(PWM_GO);
				distanceToTarget-=SPEED*elapsed;
				// advance path cursor and state to reach next way point
				if (distanceToTarget<1) {
				  if (path[pathCursor].turn()==path::STRAIGHT) {
				    advancePathCursor(pathCursor,distanceToTarget,path,state,nav,
				      start,goal);
				    pwm_steering_set(PWM_STRAIGHT);
				  } else {
				    if (path[pathCursor].turn()==path::LEFT) {
				      distanceToTarget=TURN_LEFT_DISTANCE;
				      pwm_steering_set(PWM_LEFT);
				    } else {
				      distanceToTarget=TURN_RIGHT_DISTANCE;
				      pwm_steering_set(PWM_RIGHT);
				    }
				    state=STEERING;
				  }
				}/* else {
					uint16_t position=read_line();
					if (position<LINE_LEFT) {
						pwm_steering_set(PWM_LEFT);
					} else if (position<LINE_CENTER) {
						pwm_steering_set(PWM_STRAIGHT);
					} else {
						pwm_steering_set(PWM_RIGHT);
					}
				}*/
				break;
			case STEERING:
				// execute turn and advance path cursor when the turn is complete
				distanceToTarget-=SPEED*elapsed;
				if (distanceToTarget<1) {
				  state=DRIVING;
				  pwm_steering_set(PWM_STRAIGHT);
				  advancePathCursor(pathCursor,distanceToTarget,path,state,nav,
				      start,goal);
				  if (immediateTurn!=path::STRAIGHT) {
					  immediateTurn=path::STRAIGHT;
					  distanceToTarget-=GRID_SCALE;
				  }
				}
				break;
			case REROUTE:
				// stop driving
				pwm_set(PWM_STOP);
				// find return path
				{
				  unsigned char currentNode=path[0].destination();
				  // find destination direction
				  startDirection=getDirection(map,path[1].destination(),currentNode);
				  // reset path
				  path.clear();
				  depthFirstSearch(map,start,goal,startDirection,path,blockedFrom,blockedTo);
				  // remove blocked edge markers
				  blockedFrom=blockedTo=0;
				  #ifdef SIMULATION
				  printPath(path);
				  #endif
				  pathCursor=path.size()-1;
				  distanceToTarget=getDistanceTo(path[pathCursor]);
				  // if the first waypoint requires an immediate turn then
				  // back up to allow room for turn
				  graphEdge::directionT dir = 
				    getDirection(map,currentNode,path[pathCursor].destination());
				  
				  immediateTurn=calcSteerDirection(startDirection,dir);
				  if (immediateTurn!=path::STRAIGHT) {
				    ++pathCursor;
				    distanceToTarget=REVERSE_DISTANCE;
				    state=REVERSING;
				  } else {
				    state=DRIVING;
				  }
				}
				break;
			case REVERSING:
				// reverse the required distance so that an immediate turn
				// from the current map node can be made
				pwm_set(PWM_REVERSE);
				distanceToTarget-=SPEED*elapsed;
				if (distanceToTarget<1) {
				  pwm_set(PWM_GO);
				  if (path[pathCursor].turn()==path::LEFT) {
				    distanceToTarget=TURN_LEFT_DISTANCE;
				    pwm_steering_set(PWM_LEFT);
				  } else {
				    distanceToTarget=TURN_RIGHT_DISTANCE;
				    pwm_steering_set(PWM_RIGHT);
				  }
				  state=STEERING;
				}
				break;
			default: break;
			}
		}
		#ifdef SIMULATION
		showState(state,distanceToTarget,elapsed,pathCursor);
		#endif
	}
	// END
	pwm_set(PWM_STOP);
	#ifndef SIMULATION
	while (true) {};
	#endif

	return 0;
}
