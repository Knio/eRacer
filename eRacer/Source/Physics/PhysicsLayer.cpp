/**
 * @file PhysicsLayer.cpp
 * @brief Implementation of the PhysicsLayer class
 *
 * @date 16.01.2010
 * @author: Michael Blackadar
 */

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
	std::cout << "PHYSX released" << std::endl;
}