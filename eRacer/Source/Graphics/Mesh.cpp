/**
 * @file Mesh.cpp
 * @brief Implementation of the Mesh class
 *
 * @date 07.02.2010
 * @author: Ole Rehmsen
 */

#include "Mesh.h"



namespace Graphics {

Mesh::Mesh()
:   mesh_(NULL), 
	materials_(NULL),
	textures_(NULL),
    initialized(false)
{
}

void Mesh::Init(ID3DXMesh* mesh, unsigned int nMaterials, D3DMATERIAL9* materials, IDirect3DTexture9** textures){
	assert(NULL != mesh);
	assert(NULL != materials);
	assert(NULL != textures);

	mesh_ = mesh;
	nMaterials_ = nMaterials;
	materials_ = materials;
	textures_ = textures;
	initialized = true;
}

void Mesh::Draw(IDirect3DDevice9* device) const{
	if(!initialized)
		return;

	assert(NULL != device);

    // Meshes are divided into subsets, one for each material. Render them in a loop
    for(unsigned int i = 0; i<nMaterials_; i++){
        device->SetMaterial( &materials_[i] );
        device->SetTexture(0, textures_[i]);
        
        //make sure the mesh has been initialized at this point
        assert(NULL != mesh_);

        mesh_->DrawSubset(i);
    }
}



}