#include "CarBody.h"
#include <iostream>

namespace Physics{
CarBody::CarBody(bool dynamic, float mass, const Point3& pos, const Matrix& orient){

	Matrix rot = Rotated(orient, 0.0, PI/2.0, 0.0);

	NxMaterialDesc material;
	material.restitution =		(NxReal)0.9;
	material.dynamicFriction =	(NxReal)0.3;
	material.staticFriction =	(NxReal)0.3;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	NxCapsuleShapeDesc capDesc1, capDesc2, capDesc3;

	capDesc1.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	capDesc2.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	capDesc3.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);

	//right, long capsule
	capDesc1.height = 9.03;
	capDesc1.radius = 1.2;
	capDesc1.localPose.M = Matrix_NxMat33(rot);
	capDesc1.localPose.t = NxVec3(2.2, 0, 1.11);

	//left, long capsule
	capDesc2.height = 9.03;
	capDesc2.radius = 1.2;
	capDesc2.localPose.M = Matrix_NxMat33(rot);
	capDesc2.localPose.t = NxVec3(-2.2, 0, 1.11);

	//middle, shorter capsule
	capDesc3.height = 5.0;
	capDesc3.radius = 1.0;
	capDesc3.localPose.M = Matrix_NxMat33(rot);
	capDesc3.localPose.t = NxVec3(0, 0, 0.0);

	actorDesc.shapes.pushBack(&capDesc1);
	actorDesc.shapes.pushBack(&capDesc2);
	actorDesc.shapes.pushBack(&capDesc3);
	assert(capDesc1.isValid());
	assert(capDesc2.isValid());
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
	actorDesc.globalPose.M = Matrix_NxMat33(orient);

	assert(actorDesc.isValid());
	
	CreateActor(actorDesc); 

}

CarBody::~CarBody(){
}

}