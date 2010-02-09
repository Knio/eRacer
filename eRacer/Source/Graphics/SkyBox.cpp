#include "SkyBox.h"
#include <iostream>
using namespace std;

namespace Graphics{
	
	
SkyBox::SkyBox(const Camera& camera)
: camera_(camera)
{
	
}	
	
void SkyBox::Draw(IDirect3DDevice9* device) const {
	if (!initialized) return;

	//should be far/sqrt(2), but not enough for some reason
	Matrix transform = CreateMatrix(camera_.GetPosition(), camera_.GetFar()*0.5);

    device->SetTransform(  D3DTS_WORLDMATRIX(0), &transform );

	Mesh::Draw(device);
}
	
	
}