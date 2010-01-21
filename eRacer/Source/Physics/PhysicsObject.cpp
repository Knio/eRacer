#include "PhysicsObject.h"
#include "..\Core\Math.h"

namespace Physics{

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::SetMass(float mass)
{
	Actor->setMass((NxReal)mass);
}

float PhysicsObject::GetMass()
{
	return (float)Actor->getMass();
}

NxVec3 PhysicsObject::GetPosition()
{
	return Actor->getGlobalPosition();
}
void PhysicsObject::setPosition(NxVec3 pos)
{
	Actor->setGlobalPosition(pos);
}

NxMat33 PhysicsObject::getOrientation()
{
	return Actor->getGlobalOrientation();
}
void PhysicsObject::setOrientation(NxMat33 orient)
{
	Actor->setGlobalOrientation(orient);
}

NxActor* PhysicsObject::getActor()
{
	return Actor;
}
NxVec3 PhysicsObject::getVelocity()
{

	return Actor->getLinearVelocity();
}

void PhysicsObject::setVelocity(NxVec3 vel)
{
	Actor->setLinearVelocity(vel);
}
}