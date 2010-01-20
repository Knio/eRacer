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
	void setMass(float f);

	/**
	* @brief Method that returns the mass of the physics component
	*
	* @return The mass of the component
	*/
	float getMass();

	/**
	* @breif Method that returns the orientation matrix of the Actor
	*
	* @return The orientation matrix of the Actor
	*/
	NxMat33 returnOrientationMatrix();

	/**
	* @brief Returns the actor component of the class
	*
	* @return The actor the component
	*/
	NxActor* returnActor();

protected:
	/**
	* @breif The PhysX object that is used to control behaviour
	*/
	NxActor* Actor;
};

#endif