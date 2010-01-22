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
	if(Actor->isDynamic()){
		PhysicsObject::mass = mass;
		Actor->setMass((NxReal)mass);
	}
}

float PhysicsObject::GetMass()
{
	if(Actor->isDynamic()){
		return PhysicsObject::mass = (float)Actor->getMass();
	}
	else{
		return 0;
	}
}

Vector3 PhysicsObject::GetPosition()
{
	NxVec3 v = Actor->getGlobalPosition();
	return Vector3(v.x, v.y, v.z);
}

void PhysicsObject::SetPosition(Vector3 pos)
{
	if(Actor->isDynamic()){
		NxVec3 v(pos.x, pos.y, pos.z);
		Actor->setGlobalPosition(v);
	}
}

Matrix PhysicsObject::GetOrientation()
{
	NxMat33 m = Actor->getGlobalOrientation();
	return Matrix(m(0, 0), m(0, 1), m(0, 2), 0,
				  m(1, 0), m(1, 1), m(1, 2), 0,
				  m(2, 0), m(2, 1), m(2, 2), 0,
				  0, 0, 0, 1);
}

void PhysicsObject::SetOrientation(Matrix orient)
{
	if(Actor->isDynamic()){
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
	if(Actor->isDynamic()){
		NxVec3 v = Actor->getLinearVelocity();
		return Vector3(v.x, v.y, v.z);
	}
	else{
		return Vector3(0, 0, 0);
	}
}

void PhysicsObject::SetVelocity(Vector3 vel)
{
	if(Actor->isDynamic()){
		NxVec3 v(vel.x, vel.y, vel.z);
		Actor->setLinearVelocity(v);
	}
}

bool PhysicsObject::isDynamic(){
	return dynamic = Actor->isDynamic();
}
}