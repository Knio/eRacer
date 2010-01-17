#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(){
	actor = createBox();
}

PhysicsObject::~PhysicsObject(){

}

//copied from nvidia sample docs
NxActor* PhysicsObject::createBox(){
	// Set the box starting height to 3.5m so box starts off falling onto the ground
	NxReal boxStartHeight = 3.5; 

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(0.5,0.5,0.5);
	boxDesc.localPose.t = NxVec3(0, 0, 0);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(0,boxStartHeight,0);	
	assert(actorDesc.isValid());
	NxActor *pActor = PhysicsLayer::gPhyScene->createActor(actorDesc);	
	//assert(pActor);

	// //create actor with no shapes
	//NxShape* const *shape = pActor->getShapes();
	//NxBoxShape *boxShape = shape[0]->isBox();
	//assert(boxShape);
	//pActor->releaseShape(*boxShape);

	return pActor;
}