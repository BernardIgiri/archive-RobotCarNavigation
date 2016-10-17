#define PWM_STOP	"stop"
#define PWM_GO		"go"
#define PWM_REVERSE	"reverse"
#define PWM_LEFT	"left"
#define PWM_RIGHT	"right"
#define PWM_STRAIGHT	"straight"
#define PWM_SLIGHT_LEFT "slight left"
#define PWM_SLIGHT_RIGHT "slight right"
#define TCNT1		TimeCount()
#define IR_EMITTERS_ON 1

typedef unsigned int uint16_t;
typedef unsigned short uint8_t;
#include <stdio.h>

const char *driveWheels="go";
const char *frontWheels="straight";

unsigned int timeCounter=0;

void delay_ms(int x) { //printf("\ndelay(%i)\n",x);
};

unsigned int TimeCount() { return timeCounter+=1; }

void reportDirection(graphEdge::directionT startDirection) {
  const char *heading;
  switch (startDirection) {
    case graphEdge::NORTH: heading="NORTH"; break;
    case graphEdge::SOUTH: heading="SOUTH"; break;
    case graphEdge::EAST: heading="EAST"; break;
    case graphEdge::WEST: heading="WEST"; break;
    default: heading="ERROR"; break;
  }
  printf("\n--------\nHeading %s\n--------\n",heading);
}

void printPath(pathT &path) {
	unsigned char i=path.size();
	const char * const strMap[]={"go straight","turn left","turn right","stop"};
	while (i--) { // since the path is generated in reverse, we must print it that way
		int turn=path[i].turn();
		if (i==0) { turn=3; }
		printf("Travel %i units to node %i and %s.\n",path[i].distance(),
				path[i].destination(),strMap[turn]);
	}
	printf("Done.\n");
}

unsigned int ultra_sonic() {
	const unsigned int OBSTICAL_FREQ = 300;
	const unsigned int OBSTICAL_DURATION = 13;
	
	static unsigned int nextObstical=OBSTICAL_FREQ;
	static bool obstical=false;
	if (nextObstical--<1) {
		if (obstical) {
			nextObstical=OBSTICAL_FREQ;
		} else { nextObstical=OBSTICAL_DURATION; }
		obstical=!obstical;
	}
	//printf("\nnx %u\n",nextObstical);
	return (obstical ? 1 : 9000);
}

void init_run() {
}

void pwm_set(const char *x) {
	driveWheels=x;
	//printf("Drive wheels %s\n",x);
}
void pwm_steering_set(const char *x) {
	frontWheels=x;
	//printf("steer %s\n",x);
}

void printState(int state) {
	enum s_t {
	  DONE, // done driving end program
	  LISTEN_TO_REMOTE, // waiting for input
	  WAIT_FOR_OBSTICAL, // waiting for obstical to move
	  DRIVING, // driving to next waypoint
	  STEERING, // steering
	  REROUTE, // turning around to seek next target
	  REVERSING
	};
	switch (state) {
		case DONE:  printf("State done."); break;
		case LISTEN_TO_REMOTE:  printf("Listening to remote."); break;
		case WAIT_FOR_OBSTICAL:  printf("State waiting for obstical."); break;
		case DRIVING:  printf("State driving."); break;
		case STEERING:  printf("State steering."); break;
		case REROUTE:  printf("Routing to new path."); break;
		case REVERSING:  printf("State reversing."); break;
		default: break;
	}
	printf(" DW: %s FW: %s.",driveWheels,frontWheels);
}

void showState(int state,int dist,int elapsed,int cursor) {
	//return;
	printState(state);
	printf (" Dist: %i Elapsed: %i Cursor: %i\n",dist,elapsed,cursor);
}

unsigned int read_line(unsigned int *x) { return 2000; }




enum instruction_t {
	CONTINUE,
	DRIVE_TO,
	STOP_DRIVING,
	DRIVE_FRONT_RIGHT,
	DRIVE_FRONT_LEFT,
	DRIVE_FORWARD,
	DRIVE_BACKWARD,
	DRIVE_BACK_RIGHT,
	DRIVE_BACK_LEFT,
	STOP_RUNNING
};

typedef struct {
	instruction_t instruction;
	int16_t value;
} Command;

void getCommand(Command &cmd) {
	static unsigned int count=0;
	count++;
	const unsigned int INTERVAL=50;
	cmd.value=0;
	char tmp_c=' ';
	int16_t tmp_i = 0;
	if (count==INTERVAL*1) {
		tmp_c='S';
	} else  if (count==INTERVAL*2) {
		tmp_c='G';
		tmp_i=4;
	} else  if (count==INTERVAL*3) {
		tmp_c='G';
		tmp_i=16;
	} else  if (count==INTERVAL*20) {
		tmp_c='F';
	} else  if (count==INTERVAL*21) {
		tmp_c='S';
	} else  if (count==INTERVAL*22) {
		tmp_c='Q';
	} else  if (count==INTERVAL*23) {
		tmp_c='Z';
	} else  if (count==INTERVAL*24) {
		tmp_c='C';
	} else  if (count==INTERVAL*25) {
		tmp_c='G';
		tmp_i=10;
	} else  if (count>INTERVAL*26) {
		tmp_c='T';
	}
	if (tmp_c!=' ') { printf("\nremote:%c - %i\n",tmp_c,tmp_i); }
	uint8_t result = 2;
	if(result == 2) {
		cmd.value=tmp_i;
		switch (tmp_c) {
			case 'G': cmd.instruction=DRIVE_TO;break;
			case 'S': cmd.instruction=STOP_DRIVING;break;
			case 'E': cmd.instruction=DRIVE_FRONT_RIGHT;break;
			case 'Q': cmd.instruction=DRIVE_FRONT_LEFT;break;
			case 'F': cmd.instruction=DRIVE_FORWARD;break;
			case 'B': cmd.instruction=DRIVE_BACKWARD;break;
			case 'C': cmd.instruction=DRIVE_BACK_RIGHT;break;
			case 'Z': cmd.instruction=DRIVE_BACK_LEFT;break;
			case 'T': cmd.instruction=STOP_RUNNING;break;
			default:break;
		}
	}
	if (cmd.instruction==CONTINUE) { printf("Continue driving."); }
}

void resetCommand(Command &cmd) {
	printf("Reset");
	cmd.instruction=CONTINUE;
	cmd.value=0;
}
