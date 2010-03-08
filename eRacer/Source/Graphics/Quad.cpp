/**
 * @file Quad.cpp
 * @brief Implementation of the Quad class
 *
 * @date 21.02.2010
 * @author: Ole Rehmsen
 */

#include "Quad.h"

#include "GraphicsLayer.h"

namespace Graphics {

Quad::Quad()
: initialized(false)
{
	assert(SUCCEEDED(
		GraphicsLayer::GetInstance()->GetDevice()->CreateVertexBuffer(
			4 * sizeof(Vertex),   
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_TEX1,
			D3DPOOL_DEFAULT,
			&vertexBuffer_,
			NULL)
		));
	
	Vertex* vertices;
	vertexBuffer_->Lock(0, 0, (void**)&vertices, 0);

	//top left
	vertices[0].position = Point3(-.5f,.5f,0);
	//vertices[0].color = D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	//top right
	vertices[1].position = Point3(.5f,.5f,0);
	//vertices[1].color = D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	//bottom right
	vertices[2].position = Point3(.5f,-.5f,0);
	//vertices[2].color = D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	//bottom left
	vertices[3].position = Point3(-.5f,-.5f,0);
	//vertices[3].color = D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

	vertexBuffer_->Unlock();
}

void Quad::Init(IDirect3DTexture9* texture){
	assert(NULL != texture);
	texture_ = texture;
	initialized = true;
}

void Quad::Draw(IDirect3DDevice9* device) const{
	if(!initialized)
		return;

	device->SetTransform(D3DTS_WORLDMATRIX(0), &transform_);
	device->SetStreamSource(0, vertexBuffer_, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	device->SetTexture (0, texture_);

	device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

}