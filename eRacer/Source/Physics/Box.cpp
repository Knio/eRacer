#include "Box.h"
namespace Physics{
Box::Box(bool dynamic, float mass, Point3 pos, Matrix orient, Vector3 dimensions){
	
	PhysicsObject::dynamic = dynamic;
	PhysicsObject::mass = (dynamic ? mass : 0);
	PhysicsObject::pLayer = PhysicsLayer::g_PhysicsLayer;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(dimensions.x,dimensions.y,dimensions.z);
	boxDesc.localPose.t = NxVec3(0, 0, 0);
	NxBodyDesc bodyDesc;

	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(pos.x, pos.y, pos.z);
	actorDesc.globalPose.t	= NxVec3(pos.x, 20, pos.z);	
	
	CreateActor(actorDesc);
}
Box::~Box(){

}
}