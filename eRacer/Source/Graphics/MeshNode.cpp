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
	  transform_(IDENTITY),
	  initialized(false),
	  mesh_(NULL)
{
	
}


MeshNode::~MeshNode(){
}

void MeshNode::cullRecursive(const Camera&, vector<const Renderable*>& visibleRenderables) const{
	visibleRenderables.push_back(this);
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
			mesh_->Draw(device);
			assert(SUCCEEDED(GraphicsLayer::GetInstance()->m_pEffect->EndPass()));
	}
	assert(SUCCEEDED(GraphicsLayer::GetInstance()->m_pEffect->End()));

}



void MeshNode::Init(Mesh* mesh){
	//this method can only be called once
	assert(!initialized);
	assert(NULL != mesh);
	
	mesh_ = mesh;
	
	UpdateWorldBounds();
	initialized = true;
}

}
