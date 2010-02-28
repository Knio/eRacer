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

	UpdateLocalBounds();
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

void Mesh::UpdateLocalBounds(){
	assert(NULL != mesh_);
	
	unsigned int positionOffset = -1;

	D3DVERTEXELEMENT9 vertexElement[MAX_FVF_DECL_SIZE];
	mesh_->GetDeclaration(vertexElement);

	unsigned int i=0;
	while(i<MAX_FVF_DECL_SIZE && vertexElement[i].Stream != 0xFF){
		if(D3DDECLUSAGE_POSITION==vertexElement[i].Usage){
			positionOffset = vertexElement[i].Offset;
		}
		i++;
	}
	assert(positionOffset>=0);

	unsigned char* vertices;
		
	assert(SUCCEEDED(mesh_->LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*) &vertices)));

	localBounds_.recompute(vertices, mesh_->GetNumVertices(), mesh_->GetNumBytesPerVertex());	

	mesh_->UnlockVertexBuffer();

}




}