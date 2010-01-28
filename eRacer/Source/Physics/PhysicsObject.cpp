#include "PhysicsObject.h"

namespace Physics{

PhysicsObject::PhysicsObject(bool dynamic, float mass)
{
	PhysicsObject::dynamic = dynamic;
	PhysicsObject::mass = (dynamic ? mass : 0);
}

PhysicsObject::~PhysicsObject()
{
	
}

void PhysicsObject::SetMass(float mass)
{
	if(isDynamic()){
		PhysicsObject::mass = mass;
		Actor->setMass((NxReal)mass);
	}
}

float PhysicsObject::GetMass()
{
	if(isDynamic())
		return PhysicsObject::mass = (float)Actor->getMass();
	else
		return 0;
}

Vector3 PhysicsObject::GetPosition()
{
	assert(NULL != Actor);
	NxVec3 v = Actor->getGlobalPosition();
	return Vector3(v.x, v.y, v.z);
}

void PhysicsObject::SetPosition(const Vector3 &pos)
{
	assert(isDynamic());
	Actor->setGlobalPosition(Vector3_NxVec3(pos));
}

Matrix PhysicsObject::GetOrientation()
{
	NxMat33 m = Actor->getGlobalOrientation();
	return Matrix(m(0, 0), m(0, 1), m(0, 2), 0,
				  m(1, 0), m(1, 1), m(1, 2), 0,
				  m(2, 0), m(2, 1), m(2, 2), 0,
				  0, 0, 0, 1);
}

void PhysicsObject::SetOrientation(const Matrix &orient)
{
	if(isDynamic()){
		NxMat33 m(NxVec3(orient._11, orient._12, orient._13),
				  NxVec3(orient._21, orient._22, orient._23),
				  NxVec3(orient._31, orient._32, orient._33));
		Actor->setGlobalOrientation(m);
	}
}

NxActor* PhysicsObject::GetActor()
{
	return Actor;
}

Vector3 PhysicsObject::GetVelocity()
{
	if(isDynamic())
		return NxVec3_Vector3(Actor->getLinearVelocity());
	else
		return Vector3(0, 0, 0);
}

void PhysicsObject::SetVelocity(const Vector3 &vel)
{
	assert(isDynamic());
	Actor->setLinearVelocity(Vector3_NxVec3(vel));
}

bool PhysicsObject::isDynamic(){
	return dynamic = Actor->isDynamic();
}

void PhysicsObject::CreateActor(NxActorDesc actorDesc)
{
	Actor = PhysicsLayer::g_PhysicsLayer->AddActor(actorDesc);
}


}