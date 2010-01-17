/**
 * @file PhysicsLayer.h
 * @brief Definition of the PhysicsLayer class
 *
 * @date 16.01.2010
 * @author: Michael Blackadar
 */

#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include "NxPhysics.h"


class PhysicsLayer{
public:
	/**
	 * @brief Constructor. User must call Init before using the object.
	 */
	PhysicsLayer();
	/**
	 * @brief Destructor stub.
	 */
	~PhysicsLayer();
	/**
	 * @brief Initalize the the PHYSX object.
	 */
	void Init();
	/**
	 * @brief Release the PHYSX object.
	 */
	void Shutdown();
private:
	NxPhysicsSDK* gPhysicsSDK;
};
#endif
