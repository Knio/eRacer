
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
	
	void recompute(const Point3& A, const Point3& B, const Point3& C){
		normal = -cross(B-A,C-A);
		distance = dot(A,normal);
		normalize();
	}

	void normalize() {
		float l = length(normal);
		normal/=l;
		distance/=l;
	};
};

struct Sphere {
	Sphere() : center(ORIGIN), radius(0.0f) {}

	Point3 center;
	float radius;
};

#endif // TYPES_H_
