#include "PhysicsObject.h"

namespace Physics{

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::SetMass(float f)
{
	Actor->setMass((NxReal)f);
}

float PhysicsObject::GetMass()
{
	return (float)Actor->getMass();
}

NxVec3 PhysicsObject::GetPosition()
{
	return Actor->getGlobalPosition();
}

NxMat33 PhysicsObject::ReturnOrientationMatrix()
{
	return Actor->getGlobalOrientation();
}

NxActor* PhysicsObject::ReturnActor()
{
	return Actor;
}

}