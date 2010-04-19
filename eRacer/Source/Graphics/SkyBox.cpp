/**
 * @file SkyBox.cpp
 * @brief Implementation of the SkyBox class
 *
 * @date 07.02.2010
 * @author: Ole Rehmsen
 */

#include "SkyBox.h"

#include "GraphicsLayer.h"

#include <iostream>

using namespace std;

namespace Graphics{

void SkyBox::Draw(IDirect3DDevice9* device) const {
	if (!initialized) return;
	Camera& cam = *GraphicsLayer::GetInstance().GetCamera();
	
	//should be far/sqrt(2), but not enough for some reason
	Matrix transform = CreateMatrix(cam.GetPosition(), cam.GetFar()*0.5f);

	device->SetTransform(  D3DTS_WORLDMATRIX(0), &transform );
	mesh_->Draw(device);
}	

SkyBox::SkyBox()
:	initialized(false),
	mesh_(NULL)
{
}

void SkyBox::Init(Mesh* mesh){
	mesh_ = mesh;
	initialized = true;
}

}
