/**
 * @file TriMesh.cpp
 * @brief Implementation of the Physics::TriMesh class
 *
 * @date 31.01.2010
 * @author: Ole Rehmsen
 */

#include "TriMesh.h"
#include <NXU_Streaming.h>
#include <NxCooking.h>

namespace Physics {

	TriMesh::TriMesh(){

	}

	TriMesh::~TriMesh(){
	}


	void TriMesh::Init(ID3DXMesh& mesh){
		NxTriangleMeshDesc meshDesc;
		//generate our mesh using the cooking API
		meshDesc.numTriangles = mesh.GetNumFaces();
		meshDesc.numVertices = mesh.GetNumVertices();
		meshDesc.pointStrideBytes  = mesh.GetNumBytesPerVertex();
		meshDesc.triangleStrideBytes = 3*sizeof(unsigned short);                          
		meshDesc.flags = NX_MF_16_BIT_INDICES;

		void* points;
		void* triangles;
		
		assert(SUCCEEDED(mesh.LockVertexBuffer(D3DLOCK_READONLY, &points)));
		assert(SUCCEEDED(mesh.LockIndexBuffer(D3DLOCK_READONLY, &triangles)));

		meshDesc.points = points;
		meshDesc.triangles = triangles;

		NxTriangleMeshShapeDesc meshShapeDesc;
		//meshShapeDesc.shapeFlags |= NX_SF_FEATURE_INDICES;
		meshShapeDesc.meshFlags |= NX_MESH_SMOOTH_SPHERE_COLLISIONS ;

		//perform the cooking 
		NxCookingInterface* cooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
		cooking->NxInitCooking();
		
		NXU::MemoryWriteBuffer writeBuffer;
  		assert(cooking->NxCookTriangleMesh(meshDesc, writeBuffer));
	
		NXU::MemoryReadBuffer readBuffer(writeBuffer.data);
  		meshShapeDesc.meshData = PhysicsLayer::g_PhysicsLayer->CreateTriangleMesh(readBuffer);

		NxActorDesc actorDesc;
		actorDesc.shapes.pushBack(&meshShapeDesc);
		
		CreateActor(actorDesc);
		
		assert(SUCCEEDED(mesh.UnlockIndexBuffer()));
		assert(SUCCEEDED(mesh.UnlockVertexBuffer()));

		cooking->NxCloseCooking();
	}



}