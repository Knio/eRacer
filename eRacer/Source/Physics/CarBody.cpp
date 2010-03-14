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

	NxCapsuleShapeDesc capDesc1, capDesc2;
	NxBoxShapeDesc boxDesc;

	capDesc1.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	capDesc2.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	boxDesc.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);

	//right, long capsule
	capDesc1.height = (NxReal)9.03;
	capDesc1.radius = (NxReal)1.2;
	capDesc1.localPose.M = Matrix_NxMat33(rot);
	capDesc1.localPose.t = NxVec3((NxReal)2.2, (NxReal)0, (NxReal)1.11);

	//left, long capsule
	capDesc2.height = (NxReal)9.03;
	capDesc2.radius = (NxReal)1.2;
	capDesc2.localPose.M = Matrix_NxMat33(rot);
	capDesc2.localPose.t = NxVec3((NxReal)-2.2, (NxReal)0, (NxReal)1.11);

	//middle, shorter box
	boxDesc.dimensions.set((NxReal)0.8, (NxReal)3.5, (NxReal)1.0);
	boxDesc.localPose.M = Matrix_NxMat33(rot);
	boxDesc.localPose.t = NxVec3((NxReal)0, (NxReal)0, (NxReal)0.0);

	actorDesc.shapes.pushBack(&capDesc1);
	actorDesc.shapes.pushBack(&capDesc2);
	actorDesc.shapes.pushBack(&boxDesc);
	assert(capDesc1.isValid());
	assert(capDesc2.isValid());
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
	actorDesc.globalPose.M = Matrix_NxMat33(orient);

	assert(actorDesc.isValid());
	
	CreateActor(actorDesc); 

}

CarBody::~CarBody(){
}

}