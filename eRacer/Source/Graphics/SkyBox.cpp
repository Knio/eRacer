#include <iostream>

#include "SkyBox.h"
#include "GraphicsLayer.h"

using namespace std;

namespace Graphics{
	
	
SkyBox::SkyBox() {}


void SkyBox::Draw(IDirect3DDevice9* device) const {
	if (!initialized) return;
	Camera& cam = *GraphicsLayer::GetInstance()->camera;
	
	//should be far/sqrt(2), but not enough for some reason
	Matrix transform = CreateMatrix(cam.GetPosition(), cam.GetFar()*0.5f);

  device->SetTransform(  D3DTS_WORLDMATRIX(0), &transform );
	Mesh::Draw(device);
}	
}
