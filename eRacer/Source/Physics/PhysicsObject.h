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
#include <cmath>
#include "NxPhysics.h"
#include "Core\Math.h"
#include "Convert.h"
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
	*/
	PhysicsObject();
	
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

	void SetCentreOfMass(const Point3& centre); 

	/**
	* @brief Returns the position of the physics object
	* @return The position of the physics object
	*/
	Vector3 GetPosition();

	/**
	* @brief Sets the position of the physics object. Object must be dynamic.
	* @param pos The position of the object.
	*/
	void SetPosition(const Vector3 &pos);

	/**
	* @brief Method that returns the orientation matrix of the physics object.
	*
	* @return The orientation matrix of the physics object.
	*/
	Matrix GetOrientation();
	
	void SetTransform(const Matrix &m);
	Matrix GetTransform();
	/**
	* @brief Sets the orientation of the physics object. Object must be dynamic.
	* @param orient The orientation matrix of the object.
	*/
	void SetOrientation(const Matrix &orient);

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
	void SetVelocity(const Vector3 &vel);

	/**
	* @brief Returns the angular velocity of the physics object. Will return (0, 0, 0) if object is static.
	* @return The angular velocity of the physics object.
	*/
	Vector3 GetAngVelocity();

	/**
	* @brief Sets the angular velocity of the physics object. Object must be dynamic.
	* @param vel The angular velocity of the object.
	*/
	void SetAngVelocity(const Vector3 &vel);

	/**
	* @brief Checks if the object is movable or not
	* @return The state of the object
	*/
	bool isDynamic();

	NxActor* CreateActor(const NxActorDesc& actorDesc);

	/** returns the WORLD velocity of a point in LOCAL space */
	Vector3 GetLocalPointWorldVelocity(const Point3 &pos)
	{
		return NxVec3_Vector3(Actor->getLocalPointVelocity(Vector3_NxVec3(pos)));
	}
	
	/** adds a WORLD force at a WORLD position */
	void AddWorldForceAtWorldPos(const Vector3 &force, const Point3 &pos)
	{
		return Actor->addForceAtPos(Vector3_NxVec3(force), Vector3_NxVec3(pos));
	}	
	/** adds a WORLD force at a LOCAL position */
	void AddWorldForceAtLocalPos(const Vector3 &force, const Point3 &pos)
	{
		return Actor->addForceAtLocalPos(Vector3_NxVec3(force), Vector3_NxVec3(pos));
	}
	/** adds a LOCAL force at a WORLD position */
	void AddLocalForceAtWorldPos(const Vector3 &force, const Point3 &pos)
	{
		return Actor->addLocalForceAtPos(Vector3_NxVec3(force), Vector3_NxVec3(pos));
	}
	/** adds a LOCAL force at a LOCAL position */
	void AddLocalForceAtLocalPos(const Vector3 &force, const Point3 &pos)
	{
		return Actor->addLocalForceAtLocalPos(Vector3_NxVec3(force), Vector3_NxVec3(pos));
	}
	float GetSpeed();


	void SetGroup(CollisionGroups group) { Actor->setGroup(group); }

protected:
	/**
	* @brief The PhysX object that is used to control behaviour.
	*/
	NxActor* Actor;
};
}

#endif
