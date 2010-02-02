#include "Plane.h"
namespace Physics{

Plane::Plane(float distOrigin, const Vector3& normal){

	NxMaterialDesc material;
	material.restitution = 0.5;
	material.dynamicFriction = 0.5;
	material.staticFriction = 0.5;

    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;

	planeDesc.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);

	planeDesc.d = distOrigin;
	planeDesc.normal = NxVec3(normal.x, normal.y, normal.z);
	NxActorDesc actorDesc;
	//actorDesc.density   = 10.0f;
	actorDesc.shapes.pushBack(&planeDesc);
	CreateActor(actorDesc); //->updateMassFromShapes(0, 10);

}

Plane::~Plane(){

}

}