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


	Vector3 cameraPosition = camera_.GetPosition();
	
	cout << cameraPosition << endl;

	Matrix transform = CreateMatrix(camera_.GetPosition());

    device->SetTransform(  D3DTS_WORLDMATRIX(0), &transform );

	Mesh::Draw(device);
}
	
	
}