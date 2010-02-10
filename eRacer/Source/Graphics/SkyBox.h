

#pragma once

#include "Mesh.h"
#include "Camera.h"


namespace Graphics {

class SkyBox : public Mesh {
public:	
    SkyBox(const Camera& camera);
	virtual void Draw(IDirect3DDevice9* device) const;
	void SetMesh(ID3DXMesh* mesh);
    
protected:
    const Camera& camera_;
};


inline void SkyBox::SetMesh(ID3DXMesh* mesh){
	//this method can only be called once
	assert(NULL == mesh_);
	assert(NULL != mesh);
	mesh_ = mesh;
}

}