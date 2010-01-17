#include "PhysicsLayer.h"

PhysicsLayer::PhysicsLayer(){

}

PhysicsLayer::~PhysicsLayer(){

}

void PhysicsLayer::Init(){
	gPhysicsSDK = NULL;
	NxPhysicsSDKDesc desc;
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL, desc, NULL);
	if(gPhysicsSDK == NULL) 
	{
		std::cout << "PHYSX failed to inialize" << std::endl;
	}
	else{
		std::cout << "PHYSX inialized" << std::endl;
	}
}

void PhysicsLayer::Shutdown(){
	gPhysicsSDK->release();
}