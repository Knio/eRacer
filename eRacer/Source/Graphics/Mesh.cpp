/**
 * @file Mesh.cpp
 * @brief Implementation of the Mesh class
 *
 * @date 07.02.2010
 * @author: Ole Rehmsen
 */

#include "Mesh.h"
#include "GraphicsLayer.h"
#include "IO/IO.h"

namespace Graphics {

Mesh::Mesh()
:   d3dMesh_(NULL),
	materials_(NULL),
	textures_(NULL),
	initialized(false),
	cached(false)
{
	m_colorMtrlTint = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
}

Mesh::Mesh(ID3DXMesh* mesh, D3DMATERIAL9 material, IDirect3DTexture9* texture, Vector4 meshCol ) :
	d3dMesh_(mesh), 
	nMaterials_(1)
{
	materials_ = new D3DMATERIAL9[1];
	materials_[0] = material;
	textures_ = new IDirect3DTexture9*[1];
	textures_[0] = texture;
	
	initialized = true;
	cached = false;

	localBounds.recompute(*d3dMesh_);
	m_colorMtrlTint = D3DXCOLOR( meshCol.x, meshCol.y, meshCol.z, meshCol.w );
}

Mesh::~Mesh(){
	if(initialized){
		if(!cached){
			delete [] materials_;
			d3dMesh_->Release();
		}
		delete [] textures_;
	}
}

void Mesh::Init(const CachedMesh& cachedMesh, const vector<IDirect3DTexture9*>& textures){
	assert(!initialized);
	assert(cachedMesh.IsValid());
	assert(cachedMesh.nMaterials == textures.size());

	d3dMesh_ = cachedMesh.d3dMesh;
	nMaterials_ = cachedMesh.nMaterials;
	materials_ = cachedMesh.materials;
	textures_ = new IDirect3DTexture9*[cachedMesh.nMaterials];
	for(unsigned int i = 0; i<textures.size(); i++)
		textures_[i] = textures[i];
	localBounds = cachedMesh.localBounds;
	initialized = true;
	cached = true;
}



void Mesh::Draw(IDirect3DDevice9* device) const{
	if(!initialized)
		return;

	assert(NULL != device);

    // Meshes are divided into subsets, one for each material. Render them in a loop
    for(unsigned int i = 0; i<nMaterials_; i++){
        device->SetMaterial( &materials_[i] );
        // GraphicsLayer::GetInstance()->SetTexture(0, textures_[i]);
        device->SetTexture(0, textures_[i]);
        //make sure the mesh has been initialized at this point
        assert(NULL != d3dMesh_);

        d3dMesh_->DrawSubset(i);
    }
}

}
