/**
 * @file PhysicsLayer.cpp
 * @brief Implementation of the PhysicsLayer class
 *
 * @date 16.01.2010
 * @author: Michael Blackadar
 */

#include "PhysicsLayer.h"

NxPhysicsSDK* PhysicsLayer::gPhySDK = NULL;
NxScene* PhysicsLayer::gPhyScene = NULL;

PhysicsLayer::PhysicsLayer(){
}

PhysicsLayer::~PhysicsLayer(){

}

void PhysicsLayer::Init(){
	gPhySDK = NULL;
	gPhySDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	if(gPhySDK == NULL){
		std::cout << "PHYSX failed to initialize" << std::endl;
		return;
	}
	else{
		std::cout << "PHYSX initialized" << std::endl;
	}
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NxVec3(0, -9.8, 0);
	gPhyScene = gPhySDK->createScene(sceneDesc);
	sceneDesc.simType = NX_SIMULATION_SW;

    // Create the default material
    NxMaterial* defaultMaterial = gPhyScene->getMaterialFromIndex(0); 
    defaultMaterial->setRestitution(0.5);
    defaultMaterial->setStaticFriction(0.5);
    defaultMaterial->setDynamicFriction(0.5);
}

void PhysicsLayer::Shutdown(){
	gPhySDK->release();
	std::cout << "PHYSX released" << std::endl;
}

void PhysicsLayer::AddPhyObj(PhysicsObject& phyObj){
	phyObjs.push_back(phyObj);
}