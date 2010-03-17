#include "CarBody.h"
#include <iostream>

namespace Physics{
CarBody::CarBody(float mass, const Point3& pos, const Matrix& orient){
	Matrix rot = orient*CreateMatrix(ORIGIN, PI/2.0f, X);

	NxMaterialDesc material;
	material.restitution =		(NxReal)0.9;
	material.dynamicFriction =	(NxReal)0.3;
	material.staticFriction =	(NxReal)0.3;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	//right, long capsule
	NxCapsuleShapeDesc capDesc1;
	capDesc1.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	capDesc1.height = (NxReal)9.03;
	capDesc1.radius = (NxReal)1.2;
	capDesc1.localPose.M = Matrix_NxMat33(rot);
	capDesc1.localPose.t = NxVec3((NxReal)2.2, (NxReal)0, (NxReal)1.11);
	assert(capDesc1.isValid());
	actorDesc.shapes.pushBack(&capDesc1);

	//left, long capsule
	NxCapsuleShapeDesc capDesc2;
	capDesc2.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	capDesc2.height = (NxReal)9.03;
	capDesc2.radius = (NxReal)1.2;
	capDesc2.localPose.M = Matrix_NxMat33(rot);
	capDesc2.localPose.t = NxVec3((NxReal)-2.2, (NxReal)0, (NxReal)1.11);
	assert(capDesc2.isValid());
	actorDesc.shapes.pushBack(&capDesc2);

	//middle, shorter box
	NxBoxShapeDesc boxDesc;
	boxDesc.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	boxDesc.dimensions.set((NxReal)2.0, (NxReal)0.7, (NxReal)3.2);
	boxDesc.localPose.t = NxVec3((NxReal)0, (NxReal)0, (NxReal)0.0);
	assert(boxDesc.isValid());
	actorDesc.shapes.pushBack(&boxDesc);
	
	
	NxBoxShapeDesc triggerBoxDesc;
	triggerBoxDesc.dimensions = NxVec3((NxReal)10.0, (NxReal)2.0, (NxReal)30.0);
	triggerBoxDesc.shapeFlags |= NX_TRIGGER_ON_STAY;
	triggerBoxDesc.localPose.t = NxVec3((NxReal)0.0, (NxReal)0, (NxReal)22.5);
	actorDesc.shapes.pushBack(&triggerBoxDesc);
	
	
	NxBodyDesc bodyDesc;
	bodyDesc.mass = mass;
	assert(bodyDesc.isValid());
	actorDesc.body	= &bodyDesc;
	actorDesc.globalPose.t	= NxVec3(pos.x, pos.y, pos.z);
	actorDesc.globalPose.M = Matrix_NxMat33(orient);

	assert(actorDesc.isValid());
	
	CreateActor(actorDesc); 
	
	SetGroup(CAR);
}

CarBody::~CarBody(){
}

}