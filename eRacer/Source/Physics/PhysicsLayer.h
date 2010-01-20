/**
 * @file PhysicsLayer.h
 * @brief The Physics Layer is responsible for simulating all physics objects in the scene.
 *
 * @date 16.01.2010
 * @author: Michael Blackadar
 */

#ifndef PHYSICS_LAYER_H
#define PHYSICS_LAYER_H

#include <iostream>
#include <vector>
#include "NxPhysics.h"

/**
* @breif The physics SDK object that the main game loop will use to store actors and return the results of their collisions
*/
class PhysicsLayer{
public:
	/**
	 * @brief Constructor. User must call Init before using the object.
	 */
	PhysicsLayer();
	/**
	 * @brief Destructor stub.
	 */
	~PhysicsLayer();

	/*
	* @breif Initializes the SDK instance
	*/
	void InitSDK();

	/*
	* @breif Ends the SDK
	*/
	void ReleaseSDK();

	/*
	* @breif Resets the SDK instance by calling Release and then Init
	* Must remember to reinitiaize all of the actors, materials ect.
	*/
	void ResetSDK();

	/*
	* @breif Starts the physics simulation
	*/
	void StartPhysics();

	/*
	* @breif Calculates the physics results and saves the info to the actors
	*/
	void GetPhysicsResults();

	/*
	* @breif Reads the parameters needed for the SDK from the file PhysicsInfo.txt
	*/
	void ReadParametersFromFile();

	/*
	* @breif Sets the parameters
	*/
	void SetParameters();	

	/*
	* @breif Method that adds an actor to the scene
	*
	* @param actorDesc 
	*						The physX description of the actor that the scene needs for creation
	*
	* @return A pointer to the created actor in the scene
	*/
	NxActor* AddActor(NxActorDesc actorDesc);

	/*
	* @breif Method that adds a material to the scene
	*
	* @param materialDesc 
	*						The physX description of the material that the scene needs for creation
	*
	* @return A pointer to the created material returned from the scene
	*/
	NxMaterial*	AddMaterial(NxMaterialDesc materialDesc);

	/*
	* @breif Method that returns the Material index in the scene, for the creation of actors with more detail
	*
	* @param material
	*						The physX material that the index is needed
	*
	* @return The index in the scene of the material needed for the actor
	*/
	int FindMaterialIndex(NxMaterial* material);

	/*
	* @breif Method that returns the Material index in the scene, for the creation of actors with more detail, 
	*		 This is equivalent to calling AddMaterial and FindMaterialIndex, but for human readablilty, it was reduced
	*
	* @param materialDesc 
	*						The physX description of the material that the scene needs for creation
	*
	* @return The index in the scene of the material needed for the actor
	*/
	int AddMaterialReturnIndex(NxMaterialDesc materialDesc);

	/*
	* @breif Stars the physics simulation
	*/
	void FinalizeSDK();

	/*
	* @breif Returns the instance of the scene
	*/
	NxScene* ReturnScene();

protected:
	// Physics SDK globals
	NxPhysicsSDK* gPhysicsSDK;
	NxScene* gScene;

	// Physics SDK parameters
	NxVec3 gGravity;
	NxReal skinWidth;
};
#endif
