

#pragma once

#include "Renderable.h"
#include "Model.h"


namespace Graphics {

class SkyBox : public Model, public Renderable {
public:	
	virtual void Draw(LPDIRECT3DDEVICE9 device) const;
	void SetMesh(ID3DXMesh* mesh);
};

inline void SkyBox::Draw(IDirect3DDevice9* device) const {
	/*
	//there need to be the same number of textures and materials
    assert(textures_.size()==materials_.size());
    // Meshes are divided into subsets, one for each material. Render them in a loop
    
    // set the transform

    Matrix transform = skyBox.GetTransform();

	//cout << camera.GetPosition().x << endl;
    transform*=CreateMatrix(camera.GetPosition());
    m_pd3dDevice->SetTransform(  D3DTS_WORLDMATRIX(0), &transform );
    
    //m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

    for(unsigned int i = 0; i<skyBox.Materials().size(); i++){
        m_pd3dDevice->SetMaterial( skyBox.Materials()[i]);
        m_pd3dDevice->SetTexture(0, skyBox.Textures()[i]);
        
        //make sure the mesh has been initialized at this point
        assert(NULL != skyBox.GetMesh());

        skyBox.GetMesh()->DrawSubset(i);
    }
	*/
}

void SkyBox::SetMesh(ID3DXMesh* mesh){
	//this method can only be called once
	assert(NULL == mesh_);
	assert(NULL != mesh);
	mesh_ = mesh;
}

}