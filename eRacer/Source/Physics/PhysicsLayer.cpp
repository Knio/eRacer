/**
 * @file PhysicsLayer.cpp
 * @brief Implementation of the PhysicsLayer class
 *
 * @date 16.01.2010
 * @author: Michael Blackadar
 */

#include "../Core/Consts.h"
extern Constants CONSTS;

#include "PhysicsLayer.h"

// Debugging globals
NxReal debugMode = 0.0;


PhysicsLayer::PhysicsLayer() : gScene(NULL) {
}

PhysicsLayer::~PhysicsLayer(){

}

void PhysicsLayer::InitSDK()
{
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL);
    if (!gPhysicsSDK)  
	{
		printf("SDK instance not initialized\n");
		return;
	}
}

void PhysicsLayer::ResetSDK()
{
	ReleaseSDK();
	InitSDK();
}

void PhysicsLayer::ReleaseSDK()
{
	if (gScene)
	{
		GetPhysicsResults();  // Make sure to fetchResults() before shutting down
		gPhysicsSDK->releaseScene(*gScene);
	}
	if (gPhysicsSDK)  gPhysicsSDK->release();
}

void PhysicsLayer::StartPhysics()
{
	// Update the time step
	//gDeltaTime = UpdateTime();

	// Start collision and dynamics for delta time since the last frame
    //gScene->simulate(gDeltaTime);
	gScene->flushStream();
}

void PhysicsLayer::GetPhysicsResults()
{
	// Get results from gScene->simulate(gDeltaTime)
	while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
}

void PhysicsLayer::ReadParametersFromFile()
{
}

void PhysicsLayer::SetParameters()
{
	// Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, (NxReal)0.01);

	// Set the debug visualization parameters
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, debugMode);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, debugMode);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, debugMode);

    // Create the scene
    NxSceneDesc sceneDesc;
 	sceneDesc.simType				= NX_SIMULATION_SW;
	sceneDesc.gravity               = NxVec3(CONSTS.PHYS_GRAVITY_X, CONSTS.PHYS_GRAVITY_Y, CONSTS.PHYS_GRAVITY_Y);

    gScene = gPhysicsSDK->createScene(sceneDesc);

	if(!gScene)
	{ 
		sceneDesc.simType			= NX_SIMULATION_SW; 
		gScene = gPhysicsSDK->createScene(sceneDesc);  
		if(!gScene) 
		{
			printf("scene instance not initialized\n");
			return;
		}
	}
}

NxActor* PhysicsLayer::AddActor(NxActorDesc actorDesc)
{
	return gScene->createActor(actorDesc);
}

NxMaterial* PhysicsLayer::AddMaterial(NxMaterialDesc materialDesc)
{
	return gScene->createMaterial(materialDesc);
}

int PhysicsLayer::FindMaterialIndex(NxMaterial* material)
{
	return material->getMaterialIndex();
}

int PhysicsLayer::AddMaterialReturnIndex(NxMaterialDesc materialDesc)
{
	NxMaterial* matTemp = AddMaterial(materialDesc);

	return FindMaterialIndex(matTemp);
}

void PhysicsLayer::FinalizeSDK()
{
	// Get the current time
	//UpdateTime();

	// Start the first frame of the simulation
	if (gScene)  StartPhysics();
}

NxScene* PhysicsLayer::ReturnScene()
{
	return gScene;
}