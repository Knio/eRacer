#ifndef CONSTS_H_
#define CONSTS_H_

struct Constants
{
	static Constants* g_Constants;
	float	PHYS_GRAVITY_X;
	float	PHYS_GRAVITY_Y;
	float	PHYS_GRAVITY_Z;
	int		PHYS_DEBUG_MODE;
	float	PHYS_SKIN_WIDTH;
	float	CAR_MASS;
	float	SPRING_MAGIC;
	float	DAMPING_MAGIC;
	float	MASS_CENTRE_X;
	float	MASS_CENTRE_Y;
	float	MASS_CENTRE_Z;
	float	REV_ALPHA;
	float	TURN_ALPHA;
};

#endif

