#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::setMass(float f)
{
	Actor->setMass((NxReal)f);
}

float PhysicsObject::getMass()
{
	return (float)Actor->getMass();
}

NxMat33 PhysicsObject::returnOrientationMatrix()
{
	return Actor->getGlobalOrientation();
}

NxActor* PhysicsObject::returnActor()
{
	return Actor;
}