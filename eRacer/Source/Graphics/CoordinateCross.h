
#pragma once

#include <cassert>
#include "Core/Types.h"
#include "Renderable.h"

namespace Graphics{



class CoordinateCross : public Renderable {
	struct Vertex {
		Point3 position;
		DWORD color;
	};
public:
	CoordinateCross();
	virtual void Draw(LPDIRECT3DDEVICE9 device) const;

private:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer_;

};

inline void CoordinateCross::Draw(LPDIRECT3DDEVICE9 device) const {
	Matrix m = CreateMatrix(Y);
	
	
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(255,255,255,255));
	device->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	device->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	
	
	//device->SetMaterial();
	device->SetTransform(D3DTS_WORLDMATRIX(0), &m);
	device->SetStreamSource(0, vertexBuffer_, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	device->DrawPrimitive(D3DPT_LINELIST, 0, 3);
};

inline CoordinateCross::CoordinateCross(){
	assert(SUCCEEDED(GraphicsLayer::GetInstance().GetDevice()->CreateVertexBuffer(
    6 * sizeof(Vertex),   
    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
    D3DFVF_XYZ | D3DFVF_DIFFUSE,
    D3DPOOL_DEFAULT,
    &vertexBuffer_,
    NULL
	)));
	
	Vertex* vertices;
	vertexBuffer_->Lock(0, 0, (void**)&vertices, 0);
	vertices[0].color = D3DCOLOR_COLORVALUE( 1.0f, 0.0f, 0.0f, 1.0f );
	vertices[1].color = D3DCOLOR_COLORVALUE( 1.0f, 0.0f, 0.0f, 1.0f );
	vertices[0].position = ORIGIN;
	vertices[1].position = 3*X;

	vertices[2].color = 0x7f00ff00;
	vertices[3].color = 0x7f00ff00;
	vertices[2].position = ORIGIN;
	vertices[3].position = 2*Y;

	vertices[4].color = 0x7f0000ff;
	vertices[5].color = 0x7f0000ff;
	vertices[4].position = ORIGIN;
	vertices[5].position = 1*Z;

	vertexBuffer_->Unlock();
	
};
}