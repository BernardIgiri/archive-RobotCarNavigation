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
#define DISTANCE_CONVERSION_FACTOR		0.03f
#define SPEED						1
#define WAIT_TIME						3
#define RE_ROUTE_TIMEOUT				2000
#define GRID_SCALE					98
#define TURN_LEFT_DISTANCE				210
#define TURN_RIGHT_DISTANCE			210
#define TURNING_RADIUS				3
#define REVERSE_DISTANCE				150

// NAVIGATION STATES
enum state_t {
  DONE, // done driving end program
  LISTEN_TO_REMOTE, // waiting for input
  WAIT_FOR_OBSTICAL, // WAIT_FOR_OBSTICAL for obstical to move
  DRIVING, // driving to next waypoint
  STEERING, // steering
  REROUTE, // re routing to seek next target
  REVERSING // reversing if the return path requires it
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
  const graph::pathT &path,state_t &state,
  unsigned char &start,unsigned char &goal) {
  if (pathCursor==0) {
     pwm_set(PWM_STOP);
    state=LISTEN_TO_REMOTE;
    return true;
  } else {
    --pathCursor;
    distanceToTarget+=getDistanceTo(path[pathCursor]);
    if (pathCursor==0) { distanceToTarget-=GRID_SCALE; }
    if (path[pathCursor+1].turn()!=path::STRAIGHT) {
	    #ifdef SIMULATION
	    printf("\nSubtract Radius\n");
	    #endif
      distanceToTarget-=TURNING_RADIUS;
    }
  }
  return false;
}

void followDirection(Command &command,state_t &state,unsigned char pathCursor,unsigned char &goal,unsigned char &start,
		const pathT &path) {
	//command.instruction=DRIVE_FRONT_RIGHT;
	if (DRIVE_TO==command.instruction) {
		pwm_set(PWM_STOP);
		pwm_steering_set(PWM_STRAIGHT);
		if (goal>0) {
			start=path[pathCursor].destination();
		}
		if (command.value!=start) {
			goal=command.value;
			state=REROUTE;
		}
	} else if (DRIVE_FRONT_LEFT==command.instruction) {
		pwm_set(PWM_GO);
		pwm_steering_set(PWM_LEFT);
	} else if (DRIVE_FRONT_RIGHT==command.instruction) {
		pwm_set(PWM_GO);
		pwm_steering_set(PWM_RIGHT);
	} else if (DRIVE_FORWARD==command.instruction) {
		pwm_set(PWM_GO);
		pwm_steering_set(PWM_STRAIGHT);
	} else if (DRIVE_BACKWARD==command.instruction) {
		pwm_set(PWM_REVERSE);
		pwm_steering_set(PWM_STRAIGHT);
	} else if (DRIVE_BACK_LEFT==command.instruction) {
		pwm_set(PWM_REVERSE);
		pwm_steering_set(PWM_LEFT);
	} else if (DRIVE_BACK_RIGHT==command.instruction) {
		pwm_set(PWM_REVERSE);
		pwm_steering_set(PWM_RIGHT);
	} else if (STOP_DRIVING==command.instruction) {
		pwm_set(PWM_STOP);
		pwm_steering_set(PWM_STRAIGHT);
	} else if (STOP_RUNNING==command.instruction) {
		pwm_set(PWM_STOP);
		state=DONE;
	} else {
		pwm_set(PWM_STOP);
		pwm_steering_set(PWM_STRAIGHT);
	}
	resetCommand(command);
	if (LISTEN_TO_REMOTE==state) { delay_ms(50); }
}

int main() {
	// CREATE MAP
	directedGraph &map=getMap();
	// NAVIGATE ON MAP
	state_t state=LISTEN_TO_REMOTE,priorState=LISTEN_TO_REMOTE;
	
	path::turnT immediateTurn=path::STRAIGHT;
	
	unsigned char start=4,goal=0;
	unsigned char blockedFrom=0,blockedTo=0;
	graphEdge::directionT startDirection=graphEdge::NORTH;
	
	// vector for calculated path
	graph::pathT path;
	
	unsigned char pathCursor=0;
	int distanceToTarget=0;
	
	unsigned int waitTimer=0; //,rerouteTimer=0;
	init_run();
	//uint16_t sensors[5];
	Command command;
	resetCommand(command);

	// DRIVE
	while (DONE!=state) {
		delay_ms(1);
		getCommand(command);
		// get elapsed time (doesn't work, simulate 1 ms elapsed)
		unsigned int elapsed=1;
		// check for obsticals2000
		uint16_t obsticalDistance=30000;//ultra_sonic();
		
		if (obsticalDistance<DISTANCE_TO_STOP && (state!=LISTEN_TO_REMOTE)) {
			if (state!=WAIT_FOR_OBSTICAL) {
				distanceToTarget-=SPEED*elapsed;
				// preserve old state prior to entering wait state
				priorState=state;
			}
			pwm_set(PWM_STOP);
			state=WAIT_FOR_OBSTICAL;
		} else {
			// drive or wait
			if (WAIT_FOR_OBSTICAL==state) {
				pwm_set(PWM_STOP);
				waitTimer+=elapsed;
				/*rerouteTimer+=elapsed;
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
				} else*/ if (waitTimer>WAIT_TIME) {
					waitTimer=0;
					// restore old state
					state=priorState;
					if (priorState==REVERSING) {
					  pwm_set(PWM_REVERSE);
					} else {
					  pwm_set(PWM_GO);
					}
				}
			} else if (DRIVING==state) {
				// track distance to target
				pwm_set(PWM_GO);
				distanceToTarget-=SPEED*elapsed;
				// advance path cursor and state to reach next way point
				if (distanceToTarget<1) {
				  if (command.instruction!=CONTINUE) {
				     pwm_set(PWM_STOP);
				     state=LISTEN_TO_REMOTE;
				  } else if (path[pathCursor].turn()==path::STRAIGHT) {
				    advancePathCursor(pathCursor,distanceToTarget,path,state,
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
				} else {
					/*uint16_t position = read_line(sensors);
					if (position < 200) {
						// ignore extremes
					} else if(position < 1000) {
						pwm_steering_set(PWM_SLIGHT_LEFT);
					} else if(position < 3000) {
						pwm_steering_set(PWM_STRAIGHT);
					} else if (position < 3600)  {
						pwm_steering_set(PWM_SLIGHT_RIGHT);
					} else {
						// ignore extremes
					}*/
				}
			} else if (STEERING==state) {
				// execute turn and advance path cursor when the turn is complete
				distanceToTarget-=SPEED*elapsed;
				if (distanceToTarget<1) {
				  state=DRIVING;
				  pwm_steering_set(PWM_STRAIGHT);
				  if (immediateTurn!=path::STRAIGHT) {
					  immediateTurn=path::STRAIGHT;
					  distanceToTarget-=GRID_SCALE;
				  }
				  advancePathCursor(pathCursor,distanceToTarget,path,state,
				      start,goal);
				}
			} else if (REROUTE==state) {
				// stop driving
				pwm_set(PWM_STOP);
				// find return path
				{
				  unsigned char currentNode=path[pathCursor].destination();
				  // find destination direction
				  startDirection=getDirection(map,path[pathCursor+1].destination(),currentNode);
				  // reset path
				  path.clear();
				  depthFirstSearch(map,start,goal,startDirection,path,blockedFrom,blockedTo);
				  // remove blocked edge markers
				  blockedFrom=blockedTo=0;
				  #ifdef SIMULATION
				  printf("From %i to %i\n",start,goal);
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
			} else if (REVERSING==state) {
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
			} else if (LISTEN_TO_REMOTE==state) {
				distanceToTarget=0;
				followDirection(command,state,pathCursor,goal,start,path);
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
