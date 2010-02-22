/**
 * @file Sprite.cpp
 * @brief Implementation of the Sprite class
 *
 * @date 21.02.2010
 * @author: Ole Rehmsen
 */

#include "Sprite.h"

#include "GraphicsLayer.h"

namespace Graphics {

Sprite::Sprite()
: initialized(false)
{
	assert(SUCCEEDED(
		GraphicsLayer::GetInstance()->GetDevice()->CreateVertexBuffer(
			4 * sizeof(Vertex),   
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
			D3DPOOL_DEFAULT,
			&vertexBuffer_,
			NULL)
		));
	
	Vertex* vertices;
	vertexBuffer_->Lock(0, 0, (void**)&vertices, 0);

	//top left
	vertices[0].position = Point3(-.5f,.5f,0);
	vertices[0].color = D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	//top right
	vertices[1].position = Point3(.5f,.5f,0);
	vertices[1].color = D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	//bottom right
	vertices[0].position = Point3(.5f,-.5f,0);
	vertices[0].color = D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	vertices[0].u = 1.0f;
	vertices[0].v = 1.0f;

	//bottom left
	vertices[0].position = Point3(-.5f,-.5f,0);
	vertices[0].color = D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
	vertices[0].u = 0.0f;
	vertices[0].v = 1.0f;

	vertexBuffer_->Unlock();
}

void Sprite::Init(IDirect3DTexture9* texture){
	assert(NULL != texture);
	texture_ = texture;
	initialized = true;
}

void Sprite::Draw(IDirect3DDevice9* device) const{
	if(!initialized)
		return;

	device->SetTransform(D3DTS_WORLDMATRIX(0), &transform_);
	device->SetStreamSource(0, vertexBuffer_, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	device->SetTexture (0, texture_);

	device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

}