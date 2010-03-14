/**
 * @file Mesh.h
 * @brief Definition of the Mesh class
 *
 * @date 07.02.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Renderable.h"

#include "BoundingSphere.h"

#define NOMINMAX
#include <windows.h>
#include <d3d9types.h>
#include <d3dx9mesh.h>

#include <vector>
#include <cassert>

using namespace std;


struct CachedMesh;

namespace Graphics{

/**
 * @brief Wrapper for a d3d mesh that can render itself
 */
class Mesh : public Renderable {
public:
	/**
	 * @brief Constructor. Setup a mesh in uninitialized state.
	 */
	Mesh();

	Mesh(ID3DXMesh* mesh, D3DMATERIAL9 material, IDirect3DTexture9* texture);

	virtual ~Mesh();

	/**
	 * @brief draw the mesh
	 *
	 * @param device
	 *			the Direct3D device to draw to
	 * @see Renderable::Draw
	 */
	virtual void Draw(IDirect3DDevice9* device) const;

	/**
	 * @brief getter for the mesh
	 *
	 * @return a const pointer to the mesh
	 */
	const ID3DXMesh* GetMesh() const;

	//TODO remove
	/**
	 * @brief Hack to allow physics to read mesh data
	 *
	 * This method will be removed as soon as there is another way for 
	 * physics to get to the vertex data - unfortunately, even locking
	 * the buffer for read-only access is not a constant operation in DX.
	 */
	ID3DXMesh& mesh() { return *d3dMesh_; };
	
	/**
	 * @brief initialize this mesh. Can be overidden by subclasses
	 *
	 * @param cached
	 *			a cached mesh that should be used for instantiation
	 * @param textures
	 *			a pointer to the memory location where the pointers to the textures are stored
	 */
	virtual void Init(const CachedMesh& cached, const vector<IDirect3DTexture9*>& textures);
	
	/**
	 * @brief flag to indicate whether the mesh is already initialized. 
	 *
	 * This prevents the mesh from drawing itself before it is properly loaded.
	 */
	bool initialized;
	
	/**
	 * @brief fot cleanup: If this flag is set, do not free memory when destructed
	 */
	bool cached;

	BoundingSphere localBounds;

	//friend class IO;
protected:
	
	ID3DXMesh* d3dMesh_;
	unsigned int nMaterials_;
	D3DMATERIAL9* materials_;
	IDirect3DTexture9** textures_;
};



inline const ID3DXMesh* Mesh::GetMesh() const{
	return d3dMesh_;
}






}
