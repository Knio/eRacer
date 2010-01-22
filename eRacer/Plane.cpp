#include "Plane.h"

namespace Physics{
Plane::Plane(bool dynamic, PhysicsLayer* pL, Matrix orient){
	PhysicsObject::dynamic = dynamic;
	PhysicsObject::mass = (dynamic ? mass : 0);
	PhysicsObject::pLayer = pL;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	// The actor has one shape, a box, 1m on a side
	NxPlaneShapeDesc planeDesc;

	actorDesc.shapes.pushBack(&planeDesc);
	
	CreateActor(actorDesc);
}
	Plane::~Plane()
	{
	}
}