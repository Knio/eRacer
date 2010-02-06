
#pragma once

#include <cassert>
#include "Core/Types.h"
#include "Renderable.h"

namespace Graphics{

struct Vertex {
	Point3 position;
	DWORD color;
};

class CoordinateCross : public Renderable {
public:
	CoordinateCross();
	virtual void Draw(LPDIRECT3DDEVICE9 device);

private:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer_;

};

inline void CoordinateCross::Draw(LPDIRECT3DDEVICE9 device){
	device->SetTransform(D3DTS_WORLDMATRIX(0), &IDENTITY);
	device->SetStreamSource(0, vertexBuffer_, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	device->DrawPrimitive(D3DPT_LINELIST,  0,   6);
};

inline CoordinateCross::CoordinateCross(){
	assert(SUCCEEDED(GraphicsLayer::GetInstance()->GetDevice()->CreateVertexBuffer(
    6 * sizeof(Vertex),   
    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
    D3DFVF_XYZ | D3DFVF_DIFFUSE,
    D3DPOOL_DEFAULT,
    &vertexBuffer_,
    NULL
	)));

	Vertex* vertices;
	vertexBuffer_->Lock(0, 0, (void**)&vertices, 0);
	vertices[0].position = Point3(0,0,0);
	vertices[0].color = 0xff0000;
	vertices[1].position = Point3(3,0,0);
	vertices[0].color = 0xff0000;
	vertices[2].position = Point3(0,0,0);
	vertices[0].color = 0x00ff00;
	vertices[3].position = Point3(0,2,0);
	vertices[0].color = 0x00ff00;
	vertices[4].position = Point3(0,0,0);
	vertices[0].color = 0x0000ff;
	vertices[5].position = Point3(0,0,1);
	vertices[0].color = 0x0000ff;

	vertexBuffer_->Unlock();
};
}