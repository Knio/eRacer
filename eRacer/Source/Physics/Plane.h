/**
 * @file Plane.h
 * @brief Definition of the Plane class
 *
 * @date 22.01.2010
 * @author: Michael Blackadar
 */

#ifndef PLANE_H
#define PLANE_H

#include "PhysicsObject.h"

namespace Physics{

/**
 * @brief A physics object representing an infinite plane.
 */
class Plane : public PhysicsObject{
public:
	/**
	 * @brief Constructor.
	 *
	 * @param distOrigin 
	 *			the distance of the plane from the origin - default: 0
	 * @param normal
	 *			a vector that is normal to the plane - default: (0,1,0)
	 */			
	Plane(float distOrigin = 0, const Vector3& normal = Y);
};
}
#endif 