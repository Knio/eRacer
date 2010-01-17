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
#include "NxActor.h"
#include "PhysicsLayer.h"

class PhysicsLayer;

class PhysicsObject{
public:
	PhysicsObject();
	~PhysicsObject();
private:
	NxActor* createBox();
	NxActor* actor;
};

#endif