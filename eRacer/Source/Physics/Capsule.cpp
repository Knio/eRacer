#include "Capsule.h"
#include <iostream>

namespace Physics{
Capsule::Capsule(bool dynamic, float mass, const Point3& pos, const Matrix& orient, float height, float radius){
	

	NxMaterialDesc material;
	material.restitution =		(NxReal)0.6;
	material.dynamicFriction =	(NxReal)0.1;
	material.staticFriction =	(NxReal)0.1;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	NxCapsuleShapeDesc capDesc1, capDesc2;

	capDesc1.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	capDesc1.height = height;
	capDesc1.radius = radius;
	capDesc2.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	capDesc2.height = height;
	capDesc2.radius = radius;

	capDesc1.localPose.M = Matrix_NxMat33(orient);
	capDesc1.localPose.t = NxVec3(3.0, 0, 0);
	capDesc2.localPose.M = Matrix_NxMat33(orient);
	capDesc2.localPose.t = NxVec3(-3.0, 0, 0);
	actorDesc.shapes.pushBack(&capDesc1);
	actorDesc.shapes.pushBack(&capDesc2);
	assert(capDesc1.isValid());
	assert(capDesc2.isValid());
	
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
	//actorDesc.globalPose.M = Matrix_NxMat33(orient);

	assert(actorDesc.isValid());
	
	CreateActor(actorDesc); 

}

Capsule::~Capsule(){
}

}