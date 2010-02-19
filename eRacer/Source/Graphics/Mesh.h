/**
 * @file Mesh.h
 * @brief Definition of the Mesh class
 *
 * @date 07.02.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Renderable.h"

#define NOMINMAX
#include <windows.h>
#include <d3d9types.h>
#include <d3dx9mesh.h>

#include <vector>

using namespace std;

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
	ID3DXMesh& mesh() { return *mesh_; };
	
	/**
	 * @brief setter for the mesh. Has to be implemented by subclasses
	 *
	 * It also updates the bounding volume(s)
	 *
	 * @param mesh
	 *			a pointer to the mesh for this geometry
	 */
	virtual void SetMesh(ID3DXMesh* mesh) = 0;

	/**
	 * @brief read access to the vector of materials
	 *
	 * @return read-only referece to the vector of materials
	 */
	const vector<D3DMATERIAL9*>& Materials() const;
	
	/**
	 * @brief read/write access to the vector of materials
	 * 
	 * @return read/write reference to the vector of materials
	 */
	vector<D3DMATERIAL9*>& Materials();

	/**
	 * @brief read access to the vector of textures
	 *
	 * @return read-only referece to the vector of textures
	 */
	const vector<IDirect3DTexture9*>& Textures() const;

	/**
	 * @brief read/write access to the vector of textures
	 * 
	 * @return read/write reference to the vector of textures
	 */
	vector<IDirect3DTexture9*>& Textures();

	/**
	 * @brief flag to indicate whether the mesh is already initialized. 
	 *
	 * This prevents the mesh from drawing itself before it is properly loaded.
	 */
	bool initialized;
protected:
	ID3DXMesh* mesh_;
	vector<D3DMATERIAL9*> materials_;
	vector<IDirect3DTexture9*> textures_;
};



inline const ID3DXMesh* Mesh::GetMesh() const{
	return mesh_;
}


inline const vector<D3DMATERIAL9*>& Mesh::Materials() const{
	return materials_;
}

inline vector<D3DMATERIAL9*>& Mesh::Materials(){
	return materials_;
}

inline const vector<IDirect3DTexture9*>& Mesh::Textures() const{
	return textures_;
}

inline vector<IDirect3DTexture9*>& Mesh::Textures(){
	return textures_;
}




}
