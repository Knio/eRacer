#include "SkyBox.h"


namespace Graphics{
	
	
SkyBox::SkyBox(const Camera& camera)
: camera_(camera)
{
	
}	
	
void SkyBox::Draw(IDirect3DDevice9* device) const {
	// if (!visible) return;
	Matrix transform = CreateMatrix(camera_.GetPosition());

    device->SetTransform(  D3DTS_WORLDMATRIX(0), &transform );

	Mesh::Draw(device);
}
	
	
}