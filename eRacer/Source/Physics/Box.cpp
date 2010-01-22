#include "Box.h"
namespace Physics{
Box::Box(bool dynamic, float mass, PhysicsLayer* pL, Point3 pos, Matrix orient, Vector3 dimensions){
	PhysicsObject::dynamic = dynamic;
	PhysicsObject::mass = (dynamic ? mass : 0);
	PhysicsObject::pLayer = pL;

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(dimensions.x,dimensions.y,dimensions.z);
	boxDesc.localPose.t = NxVec3(0, 0, 0);

	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.globalPose.t	= NxVec3(pos.x, pos.y, pos.z);	
	
	PhysicsObject::CreateActor(actorDesc);
}
Box::~Box(){

}
}