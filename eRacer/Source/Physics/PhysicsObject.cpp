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
	return NxMat33_Matrix(Actor->getGlobalOrientation());
}

void PhysicsObject::SetTransform(const Matrix &m)
{
	SetPosition(Vector3(m._41, m._42, m._43));
	SetOrientation(m);
}


Matrix PhysicsObject::GetTransform()
{
	NxMat33 m = Actor->getGlobalOrientation();
	NxVec3  p = Actor->getGlobalPosition();
	return Matrix(
		m(0, 0), 	m(1, 0), 	m(2, 0), 	0,
		m(0, 1), 	m(1, 1), 	m(2, 1), 	0,
		m(0, 2), 	m(1, 2), 	m(2, 2), 	0,
		p.x, 			p.y, 			p.z, 			1
	);
}

void PhysicsObject::SetOrientation(const Matrix &o)
{
	assert(isDynamic());
	Actor->setGlobalOrientation(Matrix_NxMat33(o));
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