#ifndef CONSTS_H_
#define CONSTS_H_

// These variables are populated
// from Config/eRacer.cnf by Core/Config.py

struct Constants
{
	static Constants* g_Constants;
	
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	
	
	// PhysX
	float MAX_TIMESTEP;
	float MIN_TIMESTEP;
	float SLEEP_TIME;

	float	PHYS_GRAVITY_X;
	float	PHYS_GRAVITY_Y;
	float	PHYS_GRAVITY_Z;
	
	
	float MAX_STEALING_DISTANCE;
	float STEALING_SPEED;
	
	int		PHYS_DEBUG_MODE;
	float	PHYS_SKIN_WIDTH;
	
	int		NUM_METEORS;
	float AIMED_METEOR_INTERVAL;
	
	
	int 	TRACK_DEBUG;
	
	// Vehicle
	int		CAR_DEBUG;
	int 	CAR_CPP;
	float CAR_TURN1;
	
	float CAR_DISPLACEMENT;
	float	CAR_MASS;
	float CAR_GRAVITY;
	
	float	SPRING_MAGIC;
	float	DAMPING_MAGIC;
	
	float	MASS_CENTRE_X;
	float	MASS_CENTRE_Y;
	float	MASS_CENTRE_Z;
	
	float WHEELVEL_ALPHA;
	float	REV_ALPHA;
	float	TURN_ALPHA;
	
	float	DRAG_COEFF;
	float	MAX_ENG_FORCE;
	float	MAX_BRAKE_FORCE;
	float	FRICTION_STATIC;
	float	FRICTION_SLIDING;
	float	FRICTION_ROLL;
	float BOOST_MULT;
	
};

#endif