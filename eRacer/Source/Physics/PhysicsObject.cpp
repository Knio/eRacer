#include "PhysicsObject.h"

namespace Physics{

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::~PhysicsObject()
{
	std::cout << "Removing physicsobject " << (int)(Actor->userData) << endl;
	PhysicsLayer::g_PhysicsLayer->RemoveActor(Actor);
}

void PhysicsObject::SetMass(float mass)
{
	if(IsDynamic()){
		Actor->setMass((NxReal)mass);
		Actor->updateMassFromShapes(0, mass);
	}
}

float PhysicsObject::GetMass()
{
	if(IsDynamic())
		return (float)Actor->getMass();
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
	assert(IsDynamic());
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
	assert(IsDynamic());
	Actor->setGlobalOrientation(Matrix_NxMat33(o));
}

NxActor* PhysicsObject::GetActor()
{
	return Actor;
}

Vector3 PhysicsObject::GetVelocity()
{
	if(IsDynamic())
		return NxVec3_Vector3(Actor->getLinearVelocity());
	else
		return Vector3(0, 0, 0);
}

void PhysicsObject::SetVelocity(const Vector3 &vel)
{
	assert(IsDynamic());
	Actor->setLinearVelocity(Vector3_NxVec3(vel));
}

bool PhysicsObject::IsDynamic(){
	return Actor->isDynamic();
}

NxActor* PhysicsObject::CreateActor(const NxActorDesc& actorDesc)
{
	Actor = PhysicsLayer::g_PhysicsLayer->AddActor(actorDesc);
	return Actor;
}
//centre of mass should be given in the local frame
void PhysicsObject::SetCentreOfMass(const Point3& centre){
	Actor->setCMassOffsetLocalPosition(Vector3_NxVec3(centre));
}

float PhysicsObject::GetSpeed(){
	Vector3 vel = GetVelocity();
	return sqrt(vel.x*vel.x + vel.y*vel.y + vel.z*vel.z);
}

Vector3 PhysicsObject::GetAngVelocity()
{
	if(IsDynamic())
		return NxVec3_Vector3(Actor->getAngularVelocity());
	else
		return Vector3(0, 0, 0);
}

void PhysicsObject::SetAngVelocity(const Vector3 &vel)
{
	assert(IsDynamic());
	Actor->setAngularVelocity(Vector3_NxVec3(vel));
}

}