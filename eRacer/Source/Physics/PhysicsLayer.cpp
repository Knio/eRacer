#include "PhysicsLayer.h"
extern Constants CONSTS;
namespace Physics{

PhysicsLayer* PhysicsLayer::g_PhysicsLayer = NULL;
PhysicsLayer::PhysicsLayer() : gScene(NULL) {
	g_PhysicsLayer = this;
	REGISTER(this, ReloadedConstsEvent);
}

PhysicsLayer::~PhysicsLayer(){

}

void PhysicsLayer::InitSDK()
{
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
    if (!gPhysicsSDK)  
	{
		printf("SDK instance not initialized\n");
		assert(false);
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
		gScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
		gPhysicsSDK->releaseScene(*gScene);
	}
	if (gPhysicsSDK)  gPhysicsSDK->release();
}


void PhysicsLayer::UpdatePhysics(const Time& t)
{
	float time = float(t.game_delta) / Time::RESOLUTION;
	time = std::min(time, CONSTS.PHYS_MAX_TIMESTEP);
	gScene->simulate(time);
	gScene->flushStream();
}

void PhysicsLayer::GetPhysicsResults()
{
	// Get results from gScene->simulate(gDeltaTime)
	if (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, true))
		assert(false);
}

int PhysicsLayer::ReloadedConstsEvent()
{
	if (gScene) SetupParameters();
	return 0;
}

void PhysicsLayer::SetupParameters()
{
	// Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, (NxReal)CONSTS.PHYS_SKIN_WIDTH);

	// Set the debug visualization parameters
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE,			(float)CONSTS.PHYS_DEBUG_MODE);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES,	(float)CONSTS.PHYS_DEBUG_MODE);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES,			(float)CONSTS.PHYS_DEBUG_MODE);

	//gScene->setGravity(NxVec3(CONSTS.PHYS_GRAVITY_X, CONSTS.PHYS_GRAVITY_Y, CONSTS.PHYS_GRAVITY_Z));
}

void PhysicsLayer::InitScene()
{

    // Create the scene
    NxSceneDesc sceneDesc;
 	sceneDesc.simType = NX_SIMULATION_HW;

    gScene = gPhysicsSDK->createScene(sceneDesc);

	if(!gScene)
	{ 
		sceneDesc.simType			= NX_SIMULATION_SW; 
		gScene = gPhysicsSDK->createScene(sceneDesc);  
		if(!gScene) 
		{
			printf("scene instance not initialized\n");
			assert(false);
		}
	}
	SetupParameters();
	gScene->setTiming(1.0f/500, 100, NX_TIMESTEP_VARIABLE);
	gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
}

NxActor* PhysicsLayer::AddActor(const NxActorDesc& actorDesc)
{
	return gScene->createActor(actorDesc);
}

NxTriangleMesh* PhysicsLayer::CreateTriangleMesh(const NxStream& stream){
	return gPhysicsSDK->createTriangleMesh(stream);
}

NxMaterial* PhysicsLayer::AddMaterial(const NxMaterialDesc& materialDesc)
{
	return gScene->createMaterial(materialDesc);
}

int PhysicsLayer::FindMaterialIndex(NxMaterial* material)
{
	return material->getMaterialIndex();
}

int PhysicsLayer::AddMaterialReturnIndex(const NxMaterialDesc& materialDesc)
{
	NxMaterial* matTemp = AddMaterial(materialDesc);
	assert(matTemp);
	return FindMaterialIndex(matTemp);
}

void PhysicsLayer::FinalizeSDK()
{
	if (gScene)  
	{
		UpdatePhysics(Time());
	}
	else
	{
		printf("Error loading scene\n");
		assert(false);
	}
}

NxScene* PhysicsLayer::ReturnScene()
{
	return gScene;
}

}