/**
 * @file PhysicsLayer.h
 * @brief The Physics Layer is responsible for simulating all physics objects in the scene.
 *
 * @date 16.01.2010
 * @author: Michael Blackadar
 */

#ifndef PHYSICS_LAYER_H
#define PHYSICS_LAYER_H

#include <iostream>
#include <vector>
#include "NxPhysics.h"
#include "PhysicsObject.h"
using std::vector;

class PhysicsObject;

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
	void AddPhyObj(PhysicsObject& phyObj);
	static NxPhysicsSDK* gPhySDK;
	static NxScene* gPhyScene;
private:
	vector<PhysicsObject> phyObjs;
};
#endif
