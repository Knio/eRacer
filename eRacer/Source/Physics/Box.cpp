#include "Box.h"
#include <iostream>

namespace Physics{
Box::Box(bool dynamic, float mass, const Point3& pos, const Matrix& orient, const Vector3& dimensions){
	

	NxMaterialDesc material;
	material.restitution =		(NxReal)0.6;
	material.dynamicFriction =	(NxReal)0.1;
	material.staticFriction =	(NxReal)0.1;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;


	boxDesc.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	boxDesc.dimensions.set(dimensions.x,dimensions.y,dimensions.z);
	boxDesc.localPose.t = NxVec3(0, 0, 0);
	actorDesc.shapes.pushBack(&boxDesc);
	assert(boxDesc.isValid());
	
	NxBodyDesc bodyDesc;
	if(dynamic){
		bodyDesc.mass = mass;
		assert(bodyDesc.isValid());
		actorDesc.body			= &bodyDesc;
	}
	else{
		actorDesc.body			= NULL;
	}
	actorDesc.globalPose.t	= NxVec3(pos.x, pos.y, pos.z);

	assert(actorDesc.isValid());
	
	CreateActor(actorDesc); 

}

Box::~Box(){
}
//give suspension attach positions in global space, a vector to overwrite
//returns distance to the shape, and the normal of that shape
float Box::RaycastDown(const Point3& worldsusAttachPos, Vector3& norm){
	Matrix toGlobal = GetTransform();
	Vector3 vec = mul0(toGlobal, -Y);
	normalize(vec);
	
	NxRay ray(Vector3_NxVec3(worldsusAttachPos),  Vector3_NxVec3(vec));
	
	NxScene *scene = PhysicsLayer::g_PhysicsLayer->ReturnScene();
	NxRaycastHit hit;
	NxShape* hitShape = scene->raycastClosestShape(ray, NX_STATIC_SHAPES, hit);
	norm = NxVec3_Vector3(hit.worldNormal);
	normalize(norm);
	
	return hit.distance;
}
}