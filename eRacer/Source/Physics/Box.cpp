#include "Box.h"

namespace Physics{
Box::Box(bool dynamic, float mass, Point3 pos, Matrix orient, Vector3 dimensions){
	

	NxMaterialDesc material;
	material.restitution = 0.5;
	material.dynamicFriction = 0.5;
	material.staticFriction = 0.5;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;


	boxDesc.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
	boxDesc.dimensions.set(dimensions.x,dimensions.y,dimensions.z);
	boxDesc.localPose.t = NxVec3(0, 0, 0);
	actorDesc.shapes.pushBack(&boxDesc);

	//if(dynamic){
		NxBodyDesc bodyDesc;
		actorDesc.body			= &bodyDesc;
	//}
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(pos.x, pos.y, pos.z);
	
	CreateActor(actorDesc); //->updateMassFromShapes(0, 10);
	//if(dynamic){
		SetMass(mass);
	//}
}

Box::~Box(){
}
}