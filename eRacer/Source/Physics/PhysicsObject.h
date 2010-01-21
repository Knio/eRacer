/**
 * @file PhysicsObject.h
 * @brief An object under physics simulation.
 *
 * @date 16.01.2010
 * @author: Michael Blackadar
 */

#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <iostream>
#include "NxPhysics.h"

using namespace std;

namespace Physics{
/**
* @brief Storage of an actor component that physx will use for simulation
*/
class PhysicsObject{
public:
	/**
	* @breif Constructor stub
	*/
	PhysicsObject();
	
	/**
	* @breif Destructor stub
	*/
	~PhysicsObject();

	/**
	* @brief Method that sets mass for the physics component
	*/
	void SetMass(float f);

	/**
	* @brief Method that returns the mass of the physics component
	*
	* @return The mass of the component
	*/
	float GetMass();

	NxVec3 GetPosition();

	/**
	* @breif Method that returns the orientation matrix of the Actor
	*
	* @return The orientation matrix of the Actor
	*/
	NxMat33 ReturnOrientationMatrix();

	/**
	* @brief Returns the actor component of the class
	*
	* @return The actor the component
	*/
	NxActor* ReturnActor();

protected:
	/**
	* @breif The PhysX object that is used to control behaviour
	*/
	NxActor* Actor;
};
}

#endif