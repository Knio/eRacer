/**
 * @file PhysicsObject.h
 * @brief An object under physics simulation.
 *
 * @date 16.01.2010
 * @author: Michael Blackadar, John Stuart
 */

#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <iostream>
#include "NxPhysics.h"
#include "..\Core\Math.h"

namespace Physics{
/**
* @brief Storage of an actor component that physx will use for simulation
*/
class PhysicsObject{
public:
	/**
	* @brief Constructor stub
	*/
	PhysicsObject();
	
	/**
	* @brief Destructor stub
	*/
	~PhysicsObject();

	/**
	* @brief Method that sets mass for the physics component
	* @param mass The mass in kilograms
	*/
	void SetMass(float mass);

	/**
	* @brief Method that returns the mass of the physics component
	*
	* @return The mass of the component
	*/
	float GetMass();

	/**
	* @brief Returns the position of the physics object
	* @return The position of the physics object
	*/
	Vector3 GetPosition();

	/**
	* @brief Sets the position of the physics object
	* @param pos The position of the object
	*/
	void SetPosition(Vector3 pos);

	/**
	* @brief Method that returns the orientation matrix of the Actor
	*
	* @return The orientation matrix of the Actor
	*/
	Matrix GetOrientation();

	/**
	* @brief Sets the orientation of the physics object
	* @param orient The orientation matrix of the object
	*/
	void SetOrientation(Matrix orient);

	/**
	* @brief Returns the actor component of the physics object
	*
	* @return The actor the component
	*/
	NxActor* GetActor();

	/**
	* @brief Returns the linear velocity of the physics object
	* @return The linear velocity of the physics object
	*/
	Vector3 GetVelocity();

	/**
	* @brief Sets the linear velocity of the physics object
	* @param vel The linear velocity of the object
	*/
	void SetVelocity(Vector3 vel);

protected:
	/**
	* @brief The PhysX object that is used to control behaviour
	*/
	NxActor* Actor;
};
}

#endif