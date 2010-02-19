/**
 * @file MeshNode.cpp
 * @brief Implementation of the MeshNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "MeshNode.h"
#include "GraphicsLayer.h"
#include "d3d9types.h"
#include <iostream>


using namespace std;

namespace Graphics {

MeshNode::MeshNode(const string& name)
	: Spatial(name),
	  transform_(IDENTITY)
{
	
}


MeshNode::~MeshNode(){
}

void MeshNode::cullRecursive(const Camera&, vector<const MeshNode*>& visibleNodes) const{
	visibleNodes.push_back(this);
}

void MeshNode::Draw(IDirect3DDevice9* device) const{
	if(!initialized)
		return;
			
	assert(NULL != device);

    // set the transform
	device->SetTransform(D3DTS_WORLDMATRIX(0), &transform_);

    GraphicsLayer::GetInstance()->m_pEffect->SetMatrix( "g_WorldMatrix", &transform_);
	//m_pEffect->SetTexture( "g_MeshTexture", geometry->Textures()[i] );

	assert(SUCCEEDED(GraphicsLayer::GetInstance()->m_pEffect->SetTechnique( "RenderSceneWithTextureDefault" )));
	UINT cPasses = 1;
	assert(SUCCEEDED(GraphicsLayer::GetInstance()->m_pEffect->Begin( &cPasses, 0 )));
	for(UINT iPass = 0; iPass < cPasses; iPass++ )
	{
			GraphicsLayer::GetInstance()->m_pEffect->BeginPass( iPass ) ;
			Mesh::Draw(device);
			assert(SUCCEEDED(GraphicsLayer::GetInstance()->m_pEffect->EndPass()));
	}
	assert(SUCCEEDED(GraphicsLayer::GetInstance()->m_pEffect->End()));

}


void MeshNode::UpdateBounds(){
	assert(NULL != mesh_);
	
	unsigned int bytesPerVertex = mesh_->GetNumBytesPerVertex();
	unsigned int positionOffset = -1;

	D3DVERTEXELEMENT9 vertexElement[MAX_FVF_DECL_SIZE];
	mesh_->GetDeclaration(vertexElement);

	unsigned int i=0;
	while(i<MAX_FVF_DECL_SIZE && vertexElement[i].Stream != 0xFF){
		if(D3DDECLUSAGE_POSITION==vertexElement[i].Usage){
			positionOffset = vertexElement[i].Offset;
		}
		i++;
	}
	assert(positionOffset>=0);

	unsigned char* vertices;
		
	assert(SUCCEEDED(mesh_->LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*) &vertices)));


	Point3 min, max;
	Point3 position = transformedAffine(transform_,*(Point3*)(vertices+positionOffset));
	min.x = max.x = position.x;
	min.y = max.y = position.y;
	min.z = max.z = position.z;
    
	vertices+=bytesPerVertex;
    
	for (unsigned int i=0; i<mesh_->GetNumVertices(); i++) {
		position = transformedAffine(transform_,*(Point3*)vertices);
        if(position.x < min.x)
			min.x = position.x;
		else if(position.x > max.x)
			max.x = position.x;

		if(position.y < min.y)
			min.y = position.y;
		else if(position.y > max.y)
			max.y = position.y;

		if(position.z < min.z)
			min.z = position.z;
		else if(position.z > max.z)
			max.z = position.z;

        vertices+=bytesPerVertex;
    }

	mesh_->UnlockVertexBuffer();

	worldBoundingVolume_.set(min, max);
}

void MeshNode::SetMesh(ID3DXMesh* mesh){
	//this method can only be called once
	assert(NULL == mesh_);
	
	Mesh::SetMesh(mesh);
	
	UpdateBounds();
}

}
