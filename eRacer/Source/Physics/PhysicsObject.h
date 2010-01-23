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
#include "PhysicsLayer.h"

class PhysicsLayer;

namespace Physics{
/**
* @brief Storage of an actor component that physx will use for simulation
*/
class PhysicsObject{
public:
	/**
	* @brief Constructor stub.
	* @param dynamic Whether the object can be moved or not.
	* @param mass The mass of the object in kilograms. Set as 0 for static objects.
	*/
	PhysicsObject(bool dynamic = true, float mass = 0);
	
	/**
	* @brief Destructor stub
	*/
	~PhysicsObject();

	/**
	* @brief Method that sets mass for the physics component. Object must be dynamic or nothing happens.
	* @param mass The mass in kilograms
	*/
	void SetMass(float mass);

	/**
	* @brief Method that returns the mass of the physics component. Will be 0 if the object is static.
	*
	* @return The mass of the component in kilograms
	*/
	float GetMass();

	/**
	* @brief Returns the position of the physics object
	* @return The position of the physics object
	*/
	Vector3 GetPosition();

	/**
	* @brief Sets the position of the physics object. Object must be dynamic.
	* @param pos The position of the object.
	*/
	void SetPosition(Vector3 pos);

	/**
	* @brief Method that returns the orientation matrix of the physics object.
	*
	* @return The orientation matrix of the physics object.
	*/
	Matrix GetOrientation();

	/**
	* @brief Sets the orientation of the physics object. Object must be dynamic.
	* @param orient The orientation matrix of the object.
	*/
	void SetOrientation(Matrix orient);

	/**
	* @brief Returns the actor component of the physics object.
	*
	* @return The actor component.
	*/
	NxActor* GetActor();

	/**
	* @brief Returns the linear velocity of the physics object. Will return (0, 0, 0) if object is static.
	* @return The linear velocity of the physics object.
	*/
	Vector3 GetVelocity();

	/**
	* @brief Sets the linear velocity of the physics object. Object must be dynamic.
	* @param vel The linear velocity of the object.
	*/
	void SetVelocity(Vector3 vel);

	/**
	* @brief Checks if the object is movable or not
	* @return The state of the object
	*/
	bool isDynamic();

	void CreateActor(NxActorDesc actorDesc);

protected:
	/**
	* @brief The PhysX object that is used to control behaviour.
	*/
	NxActor* Actor;
	bool dynamic;
	float mass; 
};
}

#endif