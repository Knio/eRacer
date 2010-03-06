#ifndef CONSTS_H_
#define CONSTS_H_

// These variables are populated
// from Config/eRacer.cnf by Core/Config.py

struct Constants
{
	static Constants* g_Constants;
	
	// PhysX
	float MAX_TIMESTEP;
	float MIN_TIMESTEP;
	float SLEEP_TIME;

	float	PHYS_GRAVITY_X;
	float	PHYS_GRAVITY_Y;
	float	PHYS_GRAVITY_Z;
	
	int		PHYS_DEBUG_MODE;
	float	PHYS_SKIN_WIDTH;
	
	// Vehicle
	int		CAR_DEBUG;
	int 	TRACK_DEBUG;
	
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
	float BOOST_MULT;
	
};

#endif