#include "Plane.h"
namespace Physics{

Plane::Plane(bool dynamic, float mass, float distOrigin, Vector3 normal){
	PhysicsObject::dynamic = dynamic;
	PhysicsObject::mass = (dynamic ? mass : 0);

    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;
	planeDesc.d = distOrigin;
	planeDesc.normal = NxVec3(normal.x, normal.y, normal.z);
    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
	CreateActor(actorDesc);
}

Plane::~Plane(){

}

}