
#ifndef TYPES_H_
#define TYPES_H_

#include "vect3.h"

#define PI D3DX_PI

struct Plane {
	Vector3 normal;
	float distance;
};

#endif // TYPES_H_
