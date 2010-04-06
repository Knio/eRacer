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

MeshNode::MeshNode(const string& name, const Matrix& tx)
	: RenderableNode(name,tx),
	  initialized(false),
	  mesh_(NULL),
	  boundsMesh_(NULL)
{
	
}


MeshNode::~MeshNode(){
	if(initialized)
		delete mesh_;
	if(NULL != boundsMesh_)
		delete boundsMesh_;
}


void MeshNode::Draw(IDirect3DDevice9* device) const{
	if(!initialized)
		return;
			
	assert(NULL != device);

  // set the transform
	device->SetTransform(D3DTS_WORLDMATRIX(0), &transform_);

    GraphicsLayer::GetInstance()->m_pEffect->SetMatrix( "g_WorldMatrix", &transform_);
	//m_pEffect->SetTexture( "g_MeshTexture", geometry->Textures()[i] );

	
	
	//D3DXCOLOR colorMtrlTint( 1.0f, 1.0f, 1.0f, 0.75f );
	//mesh_->m_colorMtrlTint = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	GraphicsLayer::GetInstance()->m_pEffect->SetValue( "g_ColorTint", &mesh_->m_colorMtrlTint, sizeof( D3DXCOLOR ) );

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

	if(NULL != boundsMesh_){
		Matrix t = CreateMatrix(worldBounds_.center);
		device->SetTransform(D3DTS_WORLDMATRIX(0), &t);
		boundsMesh_->DrawSubset(0);
	}

}

void MeshNode::Init(Mesh* mesh){
	//this method can only be called once
	assert(!initialized);
	assert(NULL != mesh);
	
	mesh_ = mesh;

	//D3DXCreateSphere(GraphicsLayer::GetInstance()->GetDevice(), mesh_->localBounds.radius, 10,10, &boundsMesh_,NULL);
	
	initialized = true;
	UpdateWorldBounds();
}

void MeshNode::UpdateWorldBounds(){
	if(!initialized)	
		return;
		
	worldBounds_.center = mul1(transform_, mesh_->localBounds.center);
	float x, y, z;
	ExtractScaling(transform_,x,y,z);
	worldBounds_.radius = mesh_->localBounds.radius * max(max(x,y),z);
}

}
