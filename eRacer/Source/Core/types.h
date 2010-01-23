
#ifndef TYPES_H_
#define TYPES_H_

#include <d3dx9math.h>
#include "Math.h"

/**
 * @brief A simple datastructure to store a plane
 *
 * The plane consists of all v such that dot(normal, v) = distance
 */
struct Plane {
	Vector3 normal;
	float distance;
};

#endif // TYPES_H_
