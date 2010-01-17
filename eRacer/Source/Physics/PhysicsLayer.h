#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include "NxPhysics.h"


class PhysicsLayer{
public:
	PhysicsLayer();
	~PhysicsLayer();

	void Init();
	void Shutdown();
private:
	NxPhysicsSDK* gPhysicsSDK;
};
#endif
